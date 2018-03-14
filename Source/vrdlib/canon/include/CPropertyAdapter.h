#pragma once

#include "vrdlib/api/include/IPropertySource.h"
#include "vrdlib/api/include/IConflictHandler.h"

#include <vector>
#include <string>
#include <memory>

namespace VRD
{
namespace Canon
{
   struct CPropertyAdapter : public API::IPropertySource
   {     
      CPropertyAdapter(std::unique_ptr<IPropertySource> source, API::IConflictHandler& conflictHandler);
      
      virtual OptionalPropertyType getProperty(std::string name) const override;
      virtual unsigned int foreachProperty(std::function<void(API::CProperty const&)> function) const override;
      virtual std::string toString() const override;
      
   private:
      std::unique_ptr<API::IPropertySource> m_source;
      API::IConflictHandler& m_conflictHandler;
   };   
}}
