
#include "../include/DescriptorTypes.h"

#include "vrdlib/api/include/PropertyTypes.h"
#include "vrdlib/api/include/CProperty.h"

#include <ostream>
#include <sstream>

namespace VRD
{
namespace Canon
{
   std::ostream& operator<<(std::ostream& os, DescriptorType t)  
   {
      static auto const s(API::CProperty::getNameSeparator());
      switch (t)
      {
         case DescriptorType::VRD:            os << "Canon" << s << "VRD";   break;
         case DescriptorType::Edit:           os << DescriptorType::VRD   << s << "Edit";  break;
         case DescriptorType::VRD1:           os << DescriptorType::Edit  << s << "VRD1";  break;
         case DescriptorType::VRDStampTool:   os << DescriptorType::Edit  << s << "VRDStampTool"; break;
         case DescriptorType::VRD2:           os << DescriptorType::Edit  << s << "VRD2";  break;         
         case DescriptorType::IHL:            os << DescriptorType::VRD   << s << "IHL";   break;
         case DescriptorType::XMP:            os << DescriptorType::VRD   << s << "XMP";   break;
         case DescriptorType::Edit4:          os << DescriptorType::VRD   << s << "Edit4"; break;
         case DescriptorType::DR4:            os << DescriptorType::Edit4 << s << "DR4";   break;
      }
      return os;
   }
   
   std::string to_string(DescriptorType t)
   {
      std::ostringstream os;
      os << t;
      return os.str();
   }
}}
