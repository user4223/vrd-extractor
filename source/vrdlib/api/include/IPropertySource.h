#pragma once

#include "CProperty.h"

#include <optional>
#include <string>
#include <functional>
#include <iosfwd>

namespace VRD
{
   namespace API
   {
      struct IPropertySource
      {
         typedef std::optional<CProperty> OptionalPropertyType;

         virtual ~IPropertySource() = default;

         /** Get property with given name.
       *  \param name of property to retrieve
       *  \return optional property, when it is not set, the property does not exist
       */
         virtual OptionalPropertyType getProperty(std::string name) const = 0;

         /** Visits each existing property and calls given function while passing 
       *  it as an argument.
       *  \param function to be called for each existing property
       *  \return pair of number of properties and number of propertries visited successfully
       */
         virtual std::pair<unsigned int, unsigned int> foreachProperty(std::function<bool(CProperty const &)> function) const = 0;

         virtual std::string toString() const = 0;
      };

      std::ostream &operator<<(std::ostream &os, IPropertySource const &);
   } // namespace API
} // namespace VRD
