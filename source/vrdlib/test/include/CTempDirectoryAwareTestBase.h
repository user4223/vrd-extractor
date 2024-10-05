#pragma once

#include <gtest/gtest.h>

#include <filesystem>

namespace VRD
{
   namespace Test
   {
      std::string generateRandomString(std::size_t length);

      class CTempDirectoryAwareTestBase : virtual public ::testing::Test
      {
      protected:
         CTempDirectoryAwareTestBase(std::string pathPostfix)
             : m_uniqueDirectoryPath(std::filesystem::temp_directory_path() / (generateRandomString(12) + "." + pathPostfix))
         {
         }

         virtual void SetUp() override
         {
            std::filesystem::create_directories(m_uniqueDirectoryPath);
         }

         virtual void TearDown() override
         {
            std::error_code ec;
            std::filesystem::remove_all(m_uniqueDirectoryPath, ec);
         }

         std::filesystem::path getDirectoryPath() const
         {
            return m_uniqueDirectoryPath;
         }

      private:
         std::filesystem::path m_uniqueDirectoryPath;
      };
   }
}
