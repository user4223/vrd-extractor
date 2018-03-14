#pragma once

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

namespace VRD
{
namespace Test
{
   class CTempDirectoryAwareTestBase : virtual public ::testing::Test 
   {
   protected:
      CTempDirectoryAwareTestBase(std::string pathPostfix) 
         :m_uniqueDirectoryPath(boost::filesystem::temp_directory_path() / boost::filesystem::unique_path("%%%%_%%%%_%%%%_%%%%." + pathPostfix))
      {}

      virtual void SetUp() override
      {  boost::filesystem::create_directories(m_uniqueDirectoryPath); }
      
      virtual void TearDown() override 
      {
         boost::system::error_code ec;
         boost::filesystem::remove_all(m_uniqueDirectoryPath, ec);
      }
      
      boost::filesystem::path getDirectoryPath() const
      {  return m_uniqueDirectoryPath; }
      
   private:
      boost::filesystem::path m_uniqueDirectoryPath;
   };
}}
