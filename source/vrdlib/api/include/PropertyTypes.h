#pragma once

#include <string>
#include <iosfwd>

namespace VRD
{
namespace API
{
   enum class PropertyType
   {
      Rating
   };
   
   std::string to_string(PropertyType const&);
   std::ostream& operator<<(std::ostream&, PropertyType const&);
}}
