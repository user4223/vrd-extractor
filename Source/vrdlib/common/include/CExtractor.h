#pragma once

#include "vrdlib/api/include/IConflictHandler.h"

#include <boost/filesystem/path.hpp>

#include <log4cxx/logger.h>

#include <string>
#include <memory>

namespace VRD
{
   struct CExtractor
   {
      CExtractor(boost::filesystem::path const& root, std::string whiteListRegex, std::unique_ptr<VRD::API::IConflictHandler> conflictHandler, bool dryMode);
      
   private:
      log4cxx::LoggerPtr m_logger;
      boost::filesystem::path m_root;
   };
}
