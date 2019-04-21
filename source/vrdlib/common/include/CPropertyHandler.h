#pragma once

#include "vrdlib/api/include/IPropertySource.h"
#include "vrdlib/api/include/IPropertySink.h"

#include <log4cxx/logger.h>

#include <map>

namespace VRD
{       
namespace Common
{
   struct CPropertyHandler : public API::IPropertySource, public API::IPropertySink
   {
      CPropertyHandler();
      
      virtual bool setProperty(API::CProperty property) override;
      virtual OptionalPropertyType getProperty(std::string name) const override;
      virtual std::pair<unsigned int, unsigned int> foreachProperty(std::function<bool(API::CProperty const&)> f) const override;
      virtual std::string toString() const override;
      
   private:
      mutable log4cxx::LoggerPtr m_logger;
      std::map<std::string, API::CProperty> m_properties;
   };
}}
