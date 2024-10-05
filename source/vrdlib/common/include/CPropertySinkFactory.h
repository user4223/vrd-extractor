#pragma once

#include "vrdlib/api/include/IPropertySink.h"
#include "vrdlib/api/include/IConflictHandler.h"

#include <filesystem>

#include <memory>

namespace VRD
{
   namespace Common
   {
      struct CPropertySinkFactory
      {
         CPropertySinkFactory(API::IConflictHandlerFactory &conflictHandlerFactory, API::IPropertySink::Mode mode);

         std::unique_ptr<API::IPropertySink> create(std::filesystem::path filePath);

      private:
         API::IConflictHandlerFactory &m_conflictHandlerFactory;
         API::IPropertySink::Mode m_mode;
      };
   }
}
