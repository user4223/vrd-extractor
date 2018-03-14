#pragma once

#include "vrdlib/utility/include/PositionUtility.h"

#include <string>

namespace VRD
{
namespace Utility { struct CStreamReader; }
namespace Canon
{
   struct CDirectoryEntryHeader
   {
      static uint32_t const size = 12;
      
      CDirectoryEntryHeader(Utility::CStreamReader& reader);
      
      Utility::Absolute const position;
      uint32_t const tag;
      uint32_t const unknown;
      uint32_t const payloadSize;
      std::string const description;
   };
}}
