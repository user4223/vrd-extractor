#pragma once

#include "vrdlib/api/include/IPropertySource.h"
#include "vrdlib/api/include/IConflictHandler.h"

#include <boost/filesystem/path.hpp>

#include <memory>

namespace VRD
{
namespace Common
{
   struct CPropertySourceFactory
   {
      CPropertySourceFactory(API::IConflictHandlerFactory& conflictHandlerFactory);
      
      std::unique_ptr<API::IPropertySource> create(boost::filesystem::path filePath);
      
   private:
      API::IConflictHandlerFactory& m_conflictHandlerFactory;
   };   
}}
