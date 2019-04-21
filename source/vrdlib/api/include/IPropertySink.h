#pragma once

#include "CProperty.h"

#include <string>
#include <iosfwd>

namespace VRD
{
namespace API
{
   struct IPropertySink
   {      
      enum class Mode
      {
          Normal
         ,Dry
      };
      
      virtual ~IPropertySink() = default;
      
      /** Set property with given name.
       *  \param property to be set
       */
      virtual bool setProperty(API::CProperty property) = 0;
   };
   
   std::ostream& operator<<(std::ostream&, IPropertySink::Mode mode);
}}
