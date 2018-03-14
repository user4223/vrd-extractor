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
      CPropertySourceFactory(API::IConflictHandler& conflictHandler);
      
      std::unique_ptr<API::IPropertySource> create(boost::filesystem::path filePath);
      
   private:
      API::IConflictHandler& m_conflictHandler;
   };   
}}
