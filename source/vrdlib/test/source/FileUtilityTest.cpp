
#include "vrdlib/utility/include/FileUtility.h"

#include "../include/CTempDirectoryAwareTestBase.h"

#include <gtest/gtest.h>

#include <array>
#include <regex>
#include <set>

struct FileUtilityTest : VRD::Test::CTempDirectoryAwareTestBase
{
   FileUtilityTest() : CTempDirectoryAwareTestBase("FileUtilityTest") {}

   virtual void SetUp() override
   {
      CTempDirectoryAwareTestBase::SetUp();

      static const std::array<std::string, 4> types = {"CR2", "vrd", "DR4", "BLA"};
      makeFiles(getDirectoryPath(), "", types);
      boost::filesystem::create_directories(getDirectoryPath() / "A");
      makeFiles(getDirectoryPath() / "A", "A", types);
      boost::filesystem::create_directories(getDirectoryPath() / "B" / "C");
      makeFiles(getDirectoryPath() / "B" / "C", "C", types);
   }

private:
   template <typename T>
   void makeFiles(boost::filesystem::path path, std::string _prefix, T types)
   {
      auto const prefix(_prefix.empty() ? "" : _prefix + "_");
      for (auto const &type : types)
      {
         std::ofstream((path / (prefix + "nonEmpty." + type)).string()) << 23;
         std::ofstream((path / (prefix + "empty." + type)).string());
      }
   }
};

TEST_F(FileUtilityTest, NoMatch)
{
   auto const files(VRD::Utility::getNonEmptyMatches(getDirectoryPath(), std::regex(".*blub$")));
   EXPECT_EQ(files.size(), 0u);
}

TEST_F(FileUtilityTest, MatchSingleFile)
{
   auto const files(VRD::Utility::getNonEmptyMatches(getDirectoryPath() / "A" / "A.CR2", std::regex(".*[\\/]A[\\/].*")));
   EXPECT_EQ(files.size(), 1u);
}

TEST_F(FileUtilityTest, MatchSpecificExtension)
{
   auto const files(VRD::Utility::getNonEmptyMatches(getDirectoryPath(), std::regex(".*cr2$", std::regex::icase)));
   auto const relative(boost::filesystem::relative(getDirectoryPath(), boost::filesystem::current_path()));
   EXPECT_EQ(files.size(), 3u);
   EXPECT_EQ(files.count(relative / "nonEmpty.CR2"), 1u);
   EXPECT_EQ(files.count(relative / "A" / "A_nonEmpty.CR2"), 1u);
   EXPECT_EQ(files.count(relative / "B" / "C" / "C_nonEmpty.CR2"), 1u);

   EXPECT_TRUE(boost::filesystem::exists(relative / "empty.CR2")); ///< Exists but not in selection because it's empty
}

TEST_F(FileUtilityTest, MatchAnotherSpecificExtension)
{
   auto const files(VRD::Utility::getNonEmptyMatches(getDirectoryPath(), std::regex(".*VRD$", std::regex::icase)));
   auto const relative(boost::filesystem::relative(getDirectoryPath(), boost::filesystem::current_path()));
   EXPECT_EQ(files.size(), 3u);
   EXPECT_EQ(files.count(relative / "nonEmpty.vrd"), 1u);
   EXPECT_EQ(files.count(relative / "A" / "A_nonEmpty.vrd"), 1u);
   EXPECT_EQ(files.count(relative / "B" / "C" / "C_nonEmpty.vrd"), 1u);
}

TEST_F(FileUtilityTest, MatchPathPart)
{
   auto const files(VRD::Utility::getNonEmptyMatches(getDirectoryPath(), std::regex(".*[\\/]a[\\/].*", std::regex::icase)));
   auto const relative(boost::filesystem::relative(getDirectoryPath(), boost::filesystem::current_path()));
   EXPECT_EQ(files.size(), 4u);
   EXPECT_EQ(files.count(relative / "A" / "A_nonEmpty.CR2"), 1u);
   EXPECT_EQ(files.count(relative / "A" / "A_nonEmpty.DR4"), 1u);
   EXPECT_EQ(files.count(relative / "A" / "A_nonEmpty.BLA"), 1u);
   EXPECT_EQ(files.count(relative / "A" / "A_nonEmpty.vrd"), 1u);
}
