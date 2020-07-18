#pragma once

#include <ios>

namespace VRD
{
   namespace Utility
   {
      struct Absolute
      {
         Absolute() = default;
         Absolute(Absolute const &) = default;
         Absolute(Absolute &&) = default;
         Absolute &operator=(Absolute const &) = default;
         Absolute &operator=(Absolute &&) = default;
         Absolute(std::streampos p) : m_position(p) {}
         operator std::streampos() const { return position(); }
         std::streampos position() const { return m_position; }

      private:
         std::streampos m_position;
      };

      struct Relative
      {
         Relative(Relative const &) = default;
         Relative(Relative &&) = default;
         Relative &operator=(Relative const &) = default;
         Relative &operator=(Relative &&) = default;
         Relative(std::streamoff o) : m_offset(o) {}
         operator std::streamoff() const { return offset(); }
         std::streamoff offset() const { return m_offset; }

      private:
         std::streamoff m_offset;
      };

      bool operator==(Absolute const &a, Absolute const &b);
      bool operator!=(Absolute const &a, Absolute const &b);
      bool operator<(Absolute const &a, Absolute const &b);
      bool operator>(Absolute const &a, Absolute const &b);
      Absolute operator+(Absolute a, Relative const &b);

      std::string to_string(Absolute const &);
      std::ostream &operator<<(std::ostream &, Absolute const &);
   } // namespace Utility
} // namespace VRD
