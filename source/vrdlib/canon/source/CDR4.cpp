
#include "../include/CDR4.h"

#include "../include/DescriptorTypes.h"
#include "../include/PropertyTypes.h"

#include "vrdlib/common/include/CContext.h"
#include "vrdlib/common/include/CAutoPositioner.h"

#include "vrdlib/utility/include/CStreamReader.h"

#include <vector>

namespace VRD
{
namespace Canon
{
   struct CDR4Header
   {
      static const uint32_t size = 36;
      
      /** \todo Think why the first double word might
       *        indicate little endian 'IIII' => Intel,
       *        but we have to switch to big endian to 
       *        get proper values :-?
       */
      CDR4Header(Utility::CStreamReader& reader)
         :position(        reader.getPosition())
         ,byteOrder(       reader.readRaw<uint32_t>())
         ,byteOrderToken(  reader.setByteOrderTemporarily(Utility::ByteOrder::BigEndian))
         ,unknown0(        reader.read<uint32_t>()) ///< 00040004
         ,unknown1(        reader.read<uint32_t>()) ///< 6
         ,cameraModel(     reader.read<uint32_t>())
         ,unknown2(        reader.read<uint32_t>()) ///< 3
         ,unknown3(        reader.read<uint32_t>()) ///< 4
         ,unknown4(        reader.read<uint32_t>()) ///< 5
         ,entryCount(      reader.read<uint32_t>())
         ,payloadSize(     reader.read<uint32_t>())
      {}
      
      Utility::Absolute const position;
      uint32_t const byteOrder;
      Utility::CStreamReader::Token const byteOrderToken;
      uint32_t const unknown0;
      uint32_t const unknown1;
      uint32_t const cameraModel;
      uint32_t const unknown2;
      uint32_t const unknown3;
      uint32_t const unknown4;
      uint32_t const entryCount;
      uint32_t const payloadSize;
   };
   
   struct CDR4EntryHeader
   {
      CDR4EntryHeader(Utility::CStreamReader& reader) 
         :tag(    reader.read<uint32_t>())
         ,format( reader.read<uint32_t>())
         ,flag( { reader.read<uint32_t>(), reader.read<uint32_t>(), reader.read<uint32_t>() })
         ,offset( reader.read<uint32_t>())
         ,length( reader.read<uint32_t>())
      {}
      
      uint32_t const tag;
      uint32_t const format;
      std::array<uint32_t, 3> const flag;
      uint32_t const offset;
      uint32_t const length;
   };
   
   std::ostream& operator<<(std::ostream& os, CDR4EntryHeader const& h)
   {  return os << "0x" << std::hex << h.tag << ": Format: " << std::dec << h.format << ", Offset: " << h.offset; }
   
   auto const dr4Tags([]
   {
      /** Rating offset in dr4 file: 0x967/0x169f
       */
      
      typedef std::map<uint32_t, std::string> MapType;
      MapType m;
      m.emplace(MapType::value_type(0x10101, to_string(PropertyType::DR4_CheckMark)));
      m.emplace(MapType::value_type(0x20301, to_string(PropertyType::DR4_PictureStyle)));
      m.emplace(MapType::value_type(0x20307, to_string(PropertyType::DR4_MonochromeFilterEffect)));
      m.emplace(MapType::value_type(0x20306, to_string(PropertyType::DR4_MonochromeToningEffect)));
      
      /** Add more DR4 tags here when required
       *  \see https://sno.phy.queensu.ca/~phil/exiftool/TagNames/CanonVRD.html#Edit4
       */
      return m;
   }());
   
   struct CDR4Entry : API::IInterpreter
   {     
      bool interpret(CContext& context)
      {
         Utility::CStreamReader& reader(context.getReader());
         CDR4EntryHeader const header(reader);
         
         auto const entry(dr4Tags.find(header.tag));
         if (entry == dr4Tags.end())
         {  return false; }
         
         auto const position(context.getDescriptor(to_string(DescriptorType::DR4)).getPosition());
         auto const valuePosition(position + header.offset);
         API::CProperty::ValueType value;
         switch (header.format)
         {
            case 1: 
            case 8:  value = reader.readAt<uint32_t>(valuePosition);      break;
            case 2:  value = reader.readAt<std::string>(valuePosition);   break;
            case 9:  value = reader.readAt<int32_t>(valuePosition);       break;
            case 13: value = reader.readAt<double>(valuePosition);        break;
            default:
            {  
               std::ostringstream os;
               os << "Unsupported value type found: " << header;
               throw std::domain_error(os.str()); 
            }
         }
         context.setProperty(API::CProperty(entry->second, value, to_string(valuePosition)));
         return true;
      }
   };
     
   bool CDR4::interpret(CContext& context)
   {
      auto& reader(context.getReader());
      CDR4Header const header(reader);
      
      auto const& descriptor(context.setDescriptor(CDescriptor(
         to_string(DescriptorType::DR4), header.position, header.size, header.payloadSize)));
      
      CAutoPositioner reposition(reader, descriptor);
      for (uint32_t index(0); index < header.entryCount && reader.getPosition() < descriptor.getEnd(); ++index)
      {  CDR4Entry().interpret(context); }
      return true;
   }
}}
