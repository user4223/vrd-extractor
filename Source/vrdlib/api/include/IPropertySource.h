#pragma once

#include "CProperty.h"

#include <experimental/optional>
#include <string>
#include <functional>
#include <iosfwd>

namespace VRD
{
namespace API
{
   struct IPropertySource
   {
      typedef std::experimental::optional<CProperty> OptionalPropertyType;
      
      virtual ~IPropertySource() = default;
      
      /** Get property with given name.
       *  \param name of property to retrieve
       *  \return optional property, when it is not set, the property does not exist
       */
      virtual OptionalPropertyType getProperty(std::string name) const = 0;
      
      /** Visits each existing property and calls given function while passing 
       *  it as an argument.
       *  \param function to be called for each existing property
       *  \return number of properties visited
       */
      virtual unsigned int foreachProperty(std::function<void(CProperty const&)> function) const = 0;
      
      virtual std::string toString() const = 0;
   };
   
   std::ostream& operator<<(std::ostream& os, IPropertySource const&);
}}
