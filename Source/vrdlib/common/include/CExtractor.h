#pragma once

#include <boost/filesystem/path.hpp>

#include <log4cxx/logger.h>

#include <string>

namespace VRD
{
   struct CExtractor
   {
      CExtractor(boost::filesystem::path const& root, std::string whiteListRegex, bool dryMode);
      
   private:
      log4cxx::LoggerPtr m_logger;
      boost::filesystem::path m_root;
   };
}
