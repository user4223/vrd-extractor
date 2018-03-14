#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <gtest/gtest.h>

namespace VRD
{
namespace Test
{
   struct CSampleAwareTestBase : virtual public ::testing::Test 
   {
      CSampleAwareTestBase() 
         :m_sampleDirectoryPath(boost::filesystem::canonical(boost::filesystem::path(__FILE__).parent_path().parent_path() / "etc"))
      {}
      
      boost::filesystem::path getSampleDirectory() const { return m_sampleDirectoryPath; };
       
   private:
      boost::filesystem::path m_sampleDirectoryPath;
   };
}}
