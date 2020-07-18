
#include "../include/PositionUtility.h"

#include <ostream>
#include <sstream>
#include <iomanip>

namespace VRD
{
   namespace Utility
   {
      bool operator==(Absolute const &a, Absolute const &b)
      {
         return a.position() == b.position();
      }

      bool operator!=(Absolute const &a, Absolute const &b)
      {
         return a.position() != b.position();
      }

      bool operator<(Absolute const &a, Absolute const &b)
      {
         return static_cast<size_t>(a.position()) < static_cast<size_t>(b.position());
      }

      bool operator>(Absolute const &a, Absolute const &b)
      {
         return static_cast<size_t>(a.position()) > static_cast<size_t>(b.position());
      }

      Absolute operator+(Absolute a, Relative const &b)
      {
         if (b.offset() < 0)
         {
            if (a.position() < std::abs(b.offset()))
            {
               std::ostringstream os;
               os << "Result of addition of position and negative offset would be negative: " << a << " + " << b;
               throw std::range_error(os.str());
            }
         }
         return Absolute(a.position() + b.offset());
      }

      std::string to_string(Absolute const &a)
      {
         std::ostringstream os;
         os << a;
         return os.str();
      }

      std::ostream &operator<<(std::ostream &os, Absolute const &v)
      {
         auto const p(static_cast<size_t>(v.position()));
         return os << "0x" << std::setw(8) << std::setfill('0') << std::hex << p << " (" << std::setw(8) << std::setfill('0') << std::dec << p << ")";
      }
   } // namespace Utility
} // namespace VRD
