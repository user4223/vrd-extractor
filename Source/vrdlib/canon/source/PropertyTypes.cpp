
#include "../include/PropertyTypes.h"
#include "../include/DescriptorTypes.h"

#include "vrdlib/api/include/PropertyTypes.h"
#include "vrdlib/api/include/CProperty.h"

#include <sstream>

namespace VRD
{
namespace Canon
{
   std::string to_string(PropertyType const& t)
   {
      std::ostringstream os;
      os << t;
      return os.str();
   }
   
   std::ostream& operator<<(std::ostream& os, PropertyType const& t)
   {
      static auto const s(API::CProperty::getNameSeparator());
      static auto const dr4(  to_string(DescriptorType::DR4) + s);
      static auto const vrd1( to_string(DescriptorType::VRD1) + s);
      static auto const vrd2( to_string(DescriptorType::VRD2) + s);
      static auto const xmp(  std::string("Canon") + s + "XMP" + s);
      switch (t)
      {
         case PropertyType::VRD1_CheckMark:              os << vrd1 << "CheckMark"; break;
         
         case PropertyType::VRD2_PictureStyle:           os << vrd2 << "PictureStyle"; break;
         case PropertyType::VRD2_MonochromeFilterEffect: os << vrd2 << "MonochromeFilterEffect"; break;
         case PropertyType::VRD2_MonochromeToningEffect: os << vrd2 << "MonochromeToningEffect"; break;
         case PropertyType::VRD2_CheckMark:              os << vrd2 << "CheckMark"; break;
      
         case PropertyType::DR4_CheckMark:               os << dr4 << "CheckMark"; break;
         case PropertyType::DR4_PictureStyle:            os << dr4 << "PictureStyle"; break;
         case PropertyType::DR4_MonochromeFilterEffect:  os << dr4 << "MonochromeFilterEffect"; break;
         case PropertyType::DR4_MonochromeToningEffect:  os << dr4 << "MonochromeToningEffect"; break;
         
         case PropertyType::XMP_Rating:                  os << xmp << "Rating"; break;
      }
      return os;
   }
}}
