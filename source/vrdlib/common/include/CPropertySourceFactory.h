#pragma once

#include "vrdlib/api/include/IPropertySource.h"
#include "vrdlib/api/include/IConflictHandler.h"

#include <filesystem>

#include <memory>

namespace VRD
{
   namespace Common
   {
      struct CPropertySourceFactory
      {
         CPropertySourceFactory(API::IConflictHandlerFactory &conflictHandlerFactory);

         std::unique_ptr<API::IPropertySource> create(std::filesystem::path filePath);

      private:
         API::IConflictHandlerFactory &m_conflictHandlerFactory;
      };
   }
}
