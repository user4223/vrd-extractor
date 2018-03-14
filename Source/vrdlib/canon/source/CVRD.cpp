
#include "../include/DescriptorTypes.h"
#include "../include/CVRD.h"
#include "../include/CEdit.h"
#include "../include/CEdit4.h"
#include "../include/CDirectoryEntryHeader.h"

#include "vrdlib/common/include/CContext.h"
#include "vrdlib/common/include/CAutoPositioner.h"

#include "vrdlib/utility/include/CStreamReader.h"

namespace VRD
{
namespace Canon
{
   struct CUnsupportedData : API::IInterpreter
   {
      CUnsupportedData(std::string name) : m_name(name) {}

      bool interpret(CContext& context)
      {
         auto& reader(context.getReader());
         CDirectoryEntryHeader const header(reader);
         
         auto const& descriptor(context.setDescriptor(CDescriptor(
            m_name, header.position, header.size, header.payloadSize, header.description)));
         
         CAutoPositioner reposition(reader, descriptor);      
         LOG4CXX_DEBUG(context.getLogger(), "Unsupported record found, skipping: " << m_name);
         return true;
      }
      
   private:
      std::string m_name;
   };
   
   typedef std::map<uint32_t, std::unique_ptr<API::IInterpreter>> EntryMapType;
   auto const entryMap([]()
   {
      EntryMapType m;
      m.insert(EntryMapType::value_type(0xFFFF00F4, std::make_unique<CEditData>()));
      m.insert(EntryMapType::value_type(0xFFFF00F5, std::make_unique<CUnsupportedData>(to_string(DescriptorType::IHL))));
      m.insert(EntryMapType::value_type(0xFFFF00F6, std::make_unique<CUnsupportedData>(to_string(DescriptorType::XMP))));
      m.insert(EntryMapType::value_type(0xFFFF00F7, std::make_unique<CEdit4>()));
      return m;
   }());
   
   bool CVRD::interpret(CContext& context)
   {
      static std::vector<uint8_t> const vrdBoundary({'C','A','N','O','N',' ','O','P','T','I','O','N','A','L',' ','D','A','T','A', 0});
      static std::streamoff const headerSize(28);
      static std::streamoff const footerSize(64);
      
      auto& reader(context.getReader());
      
      if (reader.getSize() < footerSize) 
      {  throw std::domain_error("Overall file size less than 64 bytes, this cannot be a valid TIFF containing any recipe data"); }
      
      reader.moveTo(Utility::Absolute(-footerSize));                 ///< Before recipe data footer
      if (reader.read(20) != vrdBoundary)
      {  
         LOG4CXX_DEBUG(context.getLogger(), "Could not find virtual recipe data (VRD)");
         return false; 
      }
      
      auto const size(static_cast<size_t>(reader.read<uint32_t>()));
      Utility::Absolute const position(reader.getSize() - headerSize - size - footerSize);
             
      reader.moveTo(position);                                       ///< Before recipe data header
      if (reader.read(20) != vrdBoundary)
      {  throw std::domain_error("Start string not found, but end string was there"); }

      if (reader.read<uint32_t>() != 0x00010000)                     ///< +24
      {  throw std::domain_error("Magic number not found, but start and end strings were there"); }

      if (reader.read<uint32_t>() != size)                           ///< +28
      {  throw std::domain_error("Data size does not match, "); }
      
      auto const& descriptor(context.setDescriptor(CDescriptor(to_string(DescriptorType::VRD), position, headerSize, size)));
      
      CAutoPositioner reposition(reader, descriptor);
      while (reader.getPosition() < descriptor.getEnd())
      {
         auto const type(reader.readAt<uint32_t>(Utility::Relative(0l)));
         auto const entry(entryMap.find(type));
         if (entry == entryMap.end())
         {  break; }
         
         entry->second->interpret(context);
      }
      return true;
   }  
}}
