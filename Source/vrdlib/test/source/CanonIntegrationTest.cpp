
#include "../include/CSampleDirectoryAwareTestBase.h"

#include "vrdlib/api/include/PropertyTypes.h"

#include "vrdlib/common/include/CContext.h"

#include "vrdlib/canon/include/PropertyTypes.h"
#include "vrdlib/canon/include/CCR2.h"
#include "vrdlib/canon/include/CCRW.h"
#include "vrdlib/canon/include/CVRD.h"
#include "vrdlib/canon/include/CDR4.h"
#include "vrdlib/canon/include/CPropertyAdapter.h"

#include "vrdlib/utility/include/FileUtility.h"
#include "vrdlib/utility/include/CStreamReader.h"
#include "vrdlib/utility/include/CConflictHandler.h"

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

/** Since I don't wanna check in the binary image files for this integration 
 *  tests, the tests on CR2s are optional! However, tests running on
 *  pure recipe files VRDs are compulsory since those are text files and 
 *  checked in.
 * 
 *  \todo Add tests for DR4 rating
 */

void printWarning() { std::cout << "\nWarning: No matching files found\n\n" << std::flush; }

struct CanonIntegrationTest : public VRD::Test::CSampleAwareTestBase
{
   CanonIntegrationTest() : CSampleAwareTestBase() {}
   virtual void SetUp() override {}
   virtual void TearDown() override {}
   unsigned int foreach(std::string fileRegex, VRD::API::IInterpreter& interpreter, std::function<void(unsigned int index, VRD::API::IPropertySource const&)> function)
   {
      auto const files(VRD::Utility::getNonEmptyMatches(getSampleDirectory(), std::regex(fileRegex)));
      if (files.empty())
      {  
         printWarning();
         return 0;
      }
      unsigned int index(0);
      for (auto const& file : files)
      {
         VRD::CContext context(std::make_unique<VRD::Utility::CStreamReader>(file.string()));
         interpreter.interpret(context);
         function(index++, context);
      }
      return files.size();
   }
};

auto const optionalCheckForce(false); ///< Turn to true for local checks agains real images

TEST_F(CanonIntegrationTest, CR2_DPP3_CheckMark_Rating_Conflict)
{
   auto const image(getSampleDirectory() / "CR2" / "DPP_V3" / "V3_CM2_Rating2.CR2");
   if (!optionalCheckForce && !boost::filesystem::exists(image))
   {
      printWarning();
      return;
   }
   auto context(std::make_unique<VRD::CContext>(std::make_unique<VRD::Utility::CStreamReader>(image)));
   VRD::Canon::CCR2().interpret(*context);
   std::stringstream stream;
   stream << "abc\n-1\n1\n23\n0\nx\n";
   VRD::Utility::CManualConflictHandler conflictHandler(stream, std::cout);
   VRD::Canon::CPropertyAdapter adapter(std::move(context), conflictHandler);
   EXPECT_EQ(4, boost::get<int16_t>(adapter.getProperty(to_string(VRD::API::PropertyType::Rating)).value().value));
   EXPECT_EQ(2, boost::get<int16_t>(adapter.getProperty(to_string(VRD::API::PropertyType::Rating)).value().value));
   EXPECT_THROW(adapter.getProperty(to_string(VRD::API::PropertyType::Rating)), std::domain_error);
}

TEST_F(CanonIntegrationTest, CR2_DPP_V3_CheckMark)
{
   VRD::Canon::CCR2 interpreter;
   auto const count(foreach(".*[/\\]DPP[_]V3[/\\]V3[_]CM\\d[.]CR2$", interpreter, [this](auto index, auto const& query)
   {
      auto const cm(index + 1);
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::VRD1_CheckMark)));
         EXPECT_TRUE(property);
         EXPECT_EQ(cm > 3 ? 0 : cm, boost::get<uint16_t>(property.value().value));
      }
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::VRD2_CheckMark)));
         EXPECT_TRUE(property);
         EXPECT_EQ(cm, boost::get<uint16_t>(property.value().value));
      }
   }));
   if (optionalCheckForce) EXPECT_EQ(count, 5u);
}

TEST_F(CanonIntegrationTest, CRW_DPP_V3_CheckMark)
{
   VRD::Canon::CCRW interpreter;
   auto const count(foreach(".*[/\\]DPP[_]V3[/\\]V3[_]CM\\d[.]CRW$", interpreter, [this](auto index, auto const& query)
   {
      auto const cm(index + 1);
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::VRD1_CheckMark)));
         EXPECT_TRUE(property);
         EXPECT_EQ(cm > 3 ? 0 : cm, boost::get<uint16_t>(property.value().value));
      }
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::VRD2_CheckMark)));
         EXPECT_TRUE(property);
         EXPECT_EQ(cm, boost::get<uint16_t>(property.value().value));
      }
   }));
   if (optionalCheckForce) EXPECT_EQ(count, 5u);
}

TEST_F(CanonIntegrationTest, CR2_DPP_V4_CheckMark)
{
   VRD::Canon::CCR2 interpreter;
   auto const count(foreach(".*[/\\]DPP[_]V4[/\\]V4[_]CM\\d[.]CR2$", interpreter, [this](auto index, auto const& query)
   {
      auto const cm(index + 1);
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::DR4_CheckMark)));
         EXPECT_TRUE(property);
         EXPECT_EQ(cm, boost::get<uint32_t>(property.value().value));
      }
   }));
   if (optionalCheckForce) EXPECT_EQ(count, 5u);
}

TEST_F(CanonIntegrationTest, CR2_DPP_V3_Rating)
{
   VRD::Canon::CCR2 interpreter;
   auto const count(foreach(".*[/\\]DPP[_]V3[/\\]V3[_]Rating.+?[.]CR2$", interpreter, [this](auto index, auto const& query)
   {
      auto const r(static_cast<int>(index)+1);
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::XMP_Rating)));
         EXPECT_TRUE(property);
         EXPECT_EQ(r > 5 ? -1 : r, boost::get<int16_t>(property.value().value));
      }
   }));
   if (optionalCheckForce) EXPECT_EQ(count, 6u);
}

/*
TEST_F(CanonIntegrationTest, CRW_DPP_V3_Rating)
{
   VRD::Canon::CCRW interpreter;
   auto const count(foreach(".*[/\\]DPP[_]V3[/\\]V3[_]Rating.+?[.]CRW$", interpreter, [this](auto index, auto const& query)
   {
      auto const r(static_cast<int>(index)+1);
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::XMP_Rating)));
         EXPECT_TRUE(property);
         EXPECT_EQ(r > 5 ? -1 : r, boost::get<int16_t>(property.value().value));
      }
   }));
   if (optionalCheckForce) EXPECT_EQ(count, 6u);
}
*/

TEST_F(CanonIntegrationTest, CR2_DPP_V4_Rating)
{
   VRD::Canon::CCR2 interpreter;
   auto const count(foreach(".*[/\\]DPP[_]V4[/\\]V4[_]Rating.+?[.]CR2$", interpreter, [this](auto index, auto const& query)
   {
      auto const r(static_cast<int>(index)+1);
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::XMP_Rating)));
         EXPECT_TRUE(property);
         EXPECT_EQ(r > 5 ? -1 : r, boost::get<int16_t>(property.value().value));
      }
   }));
   if (optionalCheckForce) EXPECT_EQ(count, 6u);
}

TEST_F(CanonIntegrationTest, CR2_Clean)
{
   VRD::Canon::CCR2 interpreter;
   auto const count(foreach(".*Clean[.]CR2$", interpreter, [this](auto index, auto const& query)
   {
      EXPECT_FALSE(query.getProperty(to_string(VRD::Canon::PropertyType::XMP_Rating)));
      EXPECT_FALSE(query.getProperty(to_string(VRD::Canon::PropertyType::VRD1_CheckMark)));
      EXPECT_FALSE(query.getProperty(to_string(VRD::Canon::PropertyType::VRD2_CheckMark)));
      EXPECT_FALSE(query.getProperty(to_string(VRD::Canon::PropertyType::DR4_CheckMark)));
   }));
   if (optionalCheckForce) EXPECT_EQ(count, 1u);
}

TEST_F(CanonIntegrationTest, CRW_Clean)
{
   VRD::Canon::CCRW interpreter;
   auto const count(foreach(".*Clean[.]CRW$", interpreter, [this](auto index, auto const& query)
   {
      EXPECT_FALSE(query.getProperty(to_string(VRD::Canon::PropertyType::XMP_Rating)));
      EXPECT_FALSE(query.getProperty(to_string(VRD::Canon::PropertyType::VRD1_CheckMark)));
      EXPECT_FALSE(query.getProperty(to_string(VRD::Canon::PropertyType::VRD2_CheckMark)));
      EXPECT_FALSE(query.getProperty(to_string(VRD::Canon::PropertyType::DR4_CheckMark)));
   }));
   if (optionalCheckForce) EXPECT_EQ(count, 1u);
}

TEST_F(CanonIntegrationTest, VRD_CheckMark)
{
   VRD::Canon::CVRD interpreter;
   auto const count(foreach(".*[/\\]DPP[_]V3[/\\]V3[_]CM\\d[.]vrd$", interpreter, [this](auto index, auto const& query)
   {
      auto const cm(index + 1);
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::VRD1_CheckMark)));
         EXPECT_TRUE(property);
         EXPECT_EQ(cm > 3 ? 0 : cm, boost::get<uint16_t>(property.value().value));
      }
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::VRD2_CheckMark)));
         EXPECT_TRUE(property);
         EXPECT_EQ(cm, boost::get<uint16_t>(property.value().value));
      }
   }));
   EXPECT_EQ(count, 5u); ///< Compulsory
}

TEST_F(CanonIntegrationTest, DR4_CheckMark)
{
   VRD::Canon::CDR4 interpreter;
   auto const count(foreach(".*[/\\]DPP[_]V4[/\\]V4[_]CM\\d[.]dr4$", interpreter, [this](auto index, auto const& query)
   {
      auto const cm(index + 1);
      {
         auto const property(query.getProperty(to_string(VRD::Canon::PropertyType::DR4_CheckMark)));
         EXPECT_TRUE(property);
         EXPECT_EQ(cm, boost::get<uint32_t>(property.value().value));
      }
   }));
   EXPECT_EQ(count, 5u); ///< Compulsory
}
