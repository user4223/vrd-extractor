#pragma once

#include <boost/filesystem/path.hpp>

namespace VRD
{
namespace Utility
{ 
   void initializeLogger(boost::filesystem::path const& configPath, boost::filesystem::path const& logFilePath);
   
   void initializeTestLogger(boost::filesystem::path const& executablePath);
}}
