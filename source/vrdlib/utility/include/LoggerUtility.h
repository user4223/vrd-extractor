#pragma once

#include <filesystem>

namespace VRD
{
   namespace Utility
   {
      void initializeLogger(std::filesystem::path const &configPath, std::filesystem::path const &logFilePath);

      void initializeTestLogger(std::filesystem::path const &executablePath);
   }
}
