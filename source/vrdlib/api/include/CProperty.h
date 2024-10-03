#pragma once

#include <variant>

#include <cstdint>
#include <string>
#include <iosfwd>

namespace VRD
{
   namespace API
   {
      struct CProperty
      {
         static std::string getNameSeparator();

         /** Extend types when required.
          */
         using ValueType = std::variant<uint32_t, uint16_t, int32_t, int16_t, std::string, float, double>;

         CProperty(std::string name, ValueType value, std::string description = std::string());

         std::string const name;
         ValueType const value;
         std::string const description;
      };

      std::string to_string(CProperty const &);
      std::ostream &operator<<(std::ostream &os, CProperty const &p);

      std::string to_string(CProperty::ValueType const &);

      bool operator==(CProperty const &, CProperty const &);
   }
}
