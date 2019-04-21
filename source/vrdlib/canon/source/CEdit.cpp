
#include "../include/CEdit.h"

#include "../include/DescriptorTypes.h"
#include "../include/PropertyTypes.h"
#include "../include/CDirectoryEntryHeader.h"

#include "vrdlib/common/include/CContext.h"
#include "vrdlib/common/include/CAutoPositioner.h"

#include "vrdlib/utility/include/CStreamReader.h"

#include <vector>

namespace VRD
{
namespace Canon
{ 
   /** Edit data has predefined types and offsets, so we do NOT need a dynamic type
    *  mimic here as for edit4 data.
    */
   template <typename ValueT>
   struct CTableEntry : API::IInterpreter
   {      
      CTableEntry(std::string name, Utility::Relative offset) : m_name(name), m_offset(offset) {}
      
      virtual bool interpret(CContext& context) override
      {  
         auto& reader(context.getReader());
         auto const position(reader.getPosition());
         auto const value(reader.readAt<ValueT>(m_offset));
         
         context.setProperty(API::CProperty(m_name, value, to_string(position + m_offset)));
         return true;
      }
      
   private:
      std::string m_name;
      Utility::Relative m_offset;
   };
   
   auto const vrd1TableEntries([]
   {
      std::vector<std::unique_ptr<API::IInterpreter>> v;
      v.emplace_back(std::make_unique<CTableEntry<uint16_t>>(to_string(PropertyType::VRD1_CheckMark), 618));
      
      /** Add more VRD1 tags here when required
       *  \see https://sno.phy.queensu.ca/~phil/exiftool/TagNames/CanonVRD.html#Ver1
       */
      return v;
   }());
   
   struct CVRD1Data : API::IInterpreter
   {      
      virtual bool interpret(CContext& context) override
      { 
         auto& reader(context.getReader());
         auto const position(reader.getPosition());
         
         auto const& descriptor(context.setDescriptor(CDescriptor(
            to_string(DescriptorType::VRD1), position, 0, 626)));
         
         CAutoPositioner reposition(reader, descriptor);
         for (auto& entry : vrd1TableEntries) { entry->interpret(context); }
         return true; 
      }
   };
   
   struct CVRDStampTool : API::IInterpreter
   {   
      virtual bool interpret(CContext& context) override
      {
         auto& reader(context.getReader());
         auto const position( reader.getPosition());
         auto const size(     reader.read<uint32_t>());
         
         auto const& descriptor(context.setDescriptor(CDescriptor(
            to_string(DescriptorType::VRDStampTool), position, 4, size)));
         
         if (size > 0)
         {
            CAutoPositioner reposition(reader, descriptor);
            
            /** Add handling for stamp tool here when required.
             */
         }
         return true;
      }
   };
   
   auto const vrd2TableEntries([]
   {
      std::vector<std::unique_ptr<API::IInterpreter>> v;
      v.emplace_back(std::make_unique<CTableEntry<uint16_t>>(to_string(PropertyType::VRD2_PictureStyle),             4));
      v.emplace_back(std::make_unique<CTableEntry< int16_t>>(to_string(PropertyType::VRD2_MonochromeFilterEffect), 116));
      v.emplace_back(std::make_unique<CTableEntry< int16_t>>(to_string(PropertyType::VRD2_MonochromeToningEffect), 118));
      v.emplace_back(std::make_unique<CTableEntry<uint16_t>>(to_string(PropertyType::VRD2_CheckMark),              284));
      
      /** Add more VRD2 tags here when required
       *  \see https://sno.phy.queensu.ca/~phil/exiftool/TagNames/CanonVRD.html#Ver2
       */
      return v;
   }());
   
   struct CVRD2Data : API::IInterpreter
   {      
      virtual bool interpret(CContext& context) override
      {         
         auto& reader(context.getReader());
         auto const position(reader.getPosition());
         auto const size([&]
         {
            auto edit(context.getDescriptor(to_string(DescriptorType::Edit)));
            auto vrd1(context.getDescriptor(to_string(DescriptorType::VRD1)));
            auto vrdStampTool(context.getDescriptor(to_string(DescriptorType::VRDStampTool)));
            return edit.getPayloadSize() - vrd1.getSize() - vrdStampTool.getSize();
         }());
         
         auto const& descriptor(context.setDescriptor(CDescriptor(
            to_string(DescriptorType::VRD2), position, 0, size)));
         
         CAutoPositioner reposition(reader, descriptor);
         for (auto& entry : vrd2TableEntries) { entry->interpret(context); }
         return true; 
      }
   };
   
   auto const editEntries([]
   {
      std::vector<std::unique_ptr<API::IInterpreter>> v;
      v.emplace_back(std::make_unique<CVRD1Data>());
      v.emplace_back(std::make_unique<CVRDStampTool>());
      v.emplace_back(std::make_unique<CVRD2Data>());
      return v;
   }());
   
   bool CEditData::interpret(CContext& context)
   {     
      auto& reader(context.getReader());
      CDirectoryEntryHeader const header(reader);
      
      auto const& descriptor(context.setDescriptor(CDescriptor(
          to_string(DescriptorType::Edit)
         ,header.position, header.size, header.payloadSize, header.description)));
      
      CAutoPositioner reposition(reader, descriptor);
      for (auto& entry : editEntries) { entry->interpret(context); }
      return true;
   }  
}}
