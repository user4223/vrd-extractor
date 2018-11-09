
#include "../include/CCRW.h"
#include "../include/CExif.h"
#include "../include/CVRD.h"

#include "vrdlib/common/include/CContext.h"

#include "vrdlib/utility/include/CStreamReader.h"

#include <stdexcept>

namespace VRD
{
namespace Canon
{
   bool CCRW::interpret(CContext& context)
   {  
      auto& reader(context.getReader());
      reader.moveToBegin();
      
      /** CRW (CIFF) file header check.
          \see https://sno.phy.queensu.ca/~phil/exiftool/canon_raw.html
       */
      
      uint16_t const byteOrder = reader.readRaw<uint16_t>();
      if (byteOrder != 0x4949) ///< Never seen different than little endian (II)
      {  throw std::domain_error("Invalid CIFF, unsupported byte order found, expected II, but was: 0x" << std::hex << byteOrder); }
      auto const byteOrderToken(reader.setByteOrderTemporarily(Utility::ByteOrder::LittleEndian));
      
      if (reader.readRaw<uint32_t>() != 26)
      {  throw std::domain_error("Invalid CIFF, unsupported header length found, expected 26"); }
      
      if (reader.read(8) != std::vector<uint8_t>({'H','E','A','P','C','C','D','R'}))
      {  throw std::domain_error("Invalid CIFF, signature not found"); }
      
      if (reader.readRaw<uint32_t>() != 0x10002)
      {  throw std::domain_error("Invalid CIFF, unsupported version found, expected 1.2"); }
            
      if (reader.readRaw<uint32_t>() != 0 || reader.readRaw<uint32_t>() != 0)
      {  throw std::domain_error("Invalid CIFF, unsupported header found"); }
      
      CVRD().interpret(context);
      CExif().interpret(context);
      return true;
   }
}}
