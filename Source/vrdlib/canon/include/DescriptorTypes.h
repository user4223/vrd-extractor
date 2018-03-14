#pragma once

#include <string>
#include <iosfwd>

namespace VRD
{
namespace Canon
{
   enum class DescriptorType
   {
       VRD      
      ,Edit
      ,VRD1
      ,VRDStampTool
      ,VRD2
      ,IHL
      ,XMP
      ,Edit4
      ,DR4
   };
   
   std::string to_string(DescriptorType);
   std::ostream& operator<<(std::ostream& os, DescriptorType);
}}
