
#include "../include/PropertyTypes.h"
#include "../include/CProperty.h"

#include <sstream>

namespace VRD
{
namespace API
{   
   std::string to_string(PropertyType const& t)
   {
      std::ostringstream os;
      os << t;
      return os.str();
   }
   
   std::ostream& operator<<(std::ostream& os, PropertyType const& t)
   {
      /** \todo Consider, we are using XMP as a target format
       *        directly, we could use some internal format and support
       *        multiple export formats in future.
       */
      static auto const s(CProperty::getNameSeparator());
      switch (t)
      {
         case PropertyType::Rating: os << "Xmp" << s << "xmp" << s << "Rating"; break;
      }
      return os;
   }
}}
