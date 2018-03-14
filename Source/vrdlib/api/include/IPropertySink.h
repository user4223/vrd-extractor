#pragma once

#include "CProperty.h"

#include <string>

namespace VRD
{
namespace API
{
   struct IPropertySink
   {      
      virtual ~IPropertySink() = default;
      
      /** Set property with given name.
       *  \param property to be set
       */
      virtual void setProperty(API::CProperty property) = 0;
   };
}}
