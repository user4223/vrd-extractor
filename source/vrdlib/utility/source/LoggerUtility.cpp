
#include "../include/LoggerUtility.h"

#include <boost/filesystem/operations.hpp>

#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/simplelayout.h>

namespace VRD
{
namespace Utility
{ 
   void initializeLogger(boost::filesystem::path const& configPath, boost::filesystem::path const& logFilePath)
   {
      setenv("logfile.path", logFilePath.string().c_str(), 1);
      log4cxx::xml::DOMConfigurator::configure(configPath.string());
   }
   
   void initializeTestLogger(boost::filesystem::path const& executablePath)
   {
      log4cxx::AppenderPtr appender(new log4cxx::FileAppender(
          log4cxx::LayoutPtr(new log4cxx::SimpleLayout())
         ,(boost::filesystem::current_path() / (executablePath.filename().string() + ".log")).string()
         ,false /* do not append */));
   
      static log4cxx::helpers::Pool pool;
      appender->activateOptions(pool);

      log4cxx::BasicConfigurator::configure(appender);
      log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getTrace());
   }   
}}
