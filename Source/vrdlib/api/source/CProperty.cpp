
#include "../include/CProperty.h"

#include <sstream>

namespace VRD
{
namespace API
{
   std::string CProperty::getNameSeparator() 
   {  return "."; }
   
   CProperty::CProperty(std::string name, ValueType value, std::string description) 
      :name(name)
      ,value(value)
      ,description(description)
   {}
   
   std::string to_string(CProperty const& p)
   {
      std::ostringstream os;
      os << p;
      return os.str();
   }
   
   std::ostream& operator<<(std::ostream& os, CProperty const& p)
   {  
      return os << p.name << ": "
                << (p.description.empty() ? std::string() : std::string("Description: " + p.description + ", "))
                << "Value: "    << p.value;
   }
   
   std::string to_string(CProperty::ValueType const& v)
   {
      std::ostringstream os;
      os << v;
      return os.str();
   }
     
   bool operator==(CProperty const& a, CProperty const& b)
   {  return a.name == b.name && a.value == b.value && a.description == b.description; }
}}
