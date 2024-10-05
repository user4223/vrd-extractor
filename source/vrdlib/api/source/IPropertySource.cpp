
#include "../include/IPropertySource.h"

#include <iostream>

namespace VRD
{
   namespace API
   {
      std::ostream &operator<<(std::ostream &os, IPropertySource const &s)
      {
         return os << s.toString();
      }
   }
}
