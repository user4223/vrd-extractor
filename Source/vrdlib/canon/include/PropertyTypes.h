#pragma once

#include <string>
#include <iosfwd>

namespace VRD
{
namespace Canon
{
   enum class PropertyType
   {
       VRD1_CheckMark
       
      ,VRD2_PictureStyle
      ,VRD2_MonochromeFilterEffect
      ,VRD2_MonochromeToningEffect
      ,VRD2_CheckMark
      
      ,DR4_CheckMark
      ,DR4_PictureStyle
      ,DR4_MonochromeFilterEffect
      ,DR4_MonochromeToningEffect
      
      ,XMP_Rating
   };
   
   std::string to_string(PropertyType const&);
   std::ostream& operator<<(std::ostream&, PropertyType const&);
}}
