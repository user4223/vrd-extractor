#pragma once

#include "vrdlib/api/include/IConflictHandler.h"

#include <filesystem>

#include <log4cxx/logger.h>

#include <string>
#include <memory>

namespace VRD
{
   struct CExtractor
   {
      CExtractor(std::filesystem::path const &root, std::string whiteListRegex, std::unique_ptr<VRD::API::IConflictHandlerFactory> conflictHandlerFactory, bool dryMode);

   private:
      log4cxx::LoggerPtr m_logger;
      std::filesystem::path m_root;
   };
}
