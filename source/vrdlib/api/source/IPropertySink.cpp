
#include "../include/IPropertySink.h"

#include <ostream>

namespace VRD
{
   namespace API
   {

      std::ostream &operator<<(std::ostream &os, IPropertySink::Mode mode)
      {
         switch (mode)
         {
         case IPropertySink::Mode::Normal:
            os << "normal";
            break;
         case IPropertySink::Mode::Dry:
            os << "dry";
            break;
         }
         return os;
      }

   }
}