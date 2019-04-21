
#include "../include/CTiff.h"

#include "vrdlib/common/include/CContext.h"

#include "vrdlib/utility/include/CStreamReader.h"

#include <map>

namespace VRD
{
   /** \see http://www.exif.org/TIFF6.pdf
    */
   
   namespace detail
   {
      std::map<uint16_t, Utility::ByteOrder> const byteOrderMap
      {
          {0x4949, Utility::ByteOrder::LittleEndian}
         ,{0x4D4D, Utility::ByteOrder::BigEndian}
      };
   }
 
   bool CTiff::interpret(CContext& context)
   {
      auto& reader(context.getReader());
      reader.moveToBegin();
      
      auto const byteOrder(detail::byteOrderMap.at(reader.readRaw<uint16_t>()));
      auto const magicNumber(reader.readRaw<uint16_t>());
      if (magicNumber != 42)
      {  throw std::domain_error("Invalid TIFF, magic number 42 not found"); }
           
      reader.moveTo(Utility::Absolute(-2));
      auto const endTag(reader.readRaw<uint16_t>());
      if (endTag != 0xD9FF)
      {  throw std::domain_error("Invalid TIFF, end tag not found"); }
      
      reader.setByteOrder(byteOrder);
      return true;
   }  
}
