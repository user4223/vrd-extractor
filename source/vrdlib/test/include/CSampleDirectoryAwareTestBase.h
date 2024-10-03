#pragma once

#include <filesystem>

#include <gtest/gtest.h>

namespace VRD
{
   namespace Test
   {
      struct CSampleAwareTestBase : virtual public ::testing::Test
      {
         CSampleAwareTestBase()
             : m_sampleDirectoryPath(std::filesystem::canonical(std::filesystem::path(__FILE__).parent_path().parent_path() / "etc"))
         {
         }

         std::filesystem::path getSampleDirectory() const { return m_sampleDirectoryPath; };

      private:
         std::filesystem::path m_sampleDirectoryPath;
      };
   }
}
