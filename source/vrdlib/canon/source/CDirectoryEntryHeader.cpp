
#include "../include/CDirectoryEntryHeader.h"

#include "vrdlib/utility/include/CStreamReader.h"

#include <sstream>

namespace VRD
{
namespace Canon
{
   CDirectoryEntryHeader::CDirectoryEntryHeader(Utility::CStreamReader& reader)
      :position(    reader.getPosition())
      ,tag(         reader.read<uint32_t>())
      ,unknown(     reader.read<uint32_t>())
      ,payloadSize( reader.read<uint32_t>())
      ,description([this]
       {
          std::ostringstream os;
          os << "0x" << std::hex << tag;
          return os.str();
       }())
   {}
}}
