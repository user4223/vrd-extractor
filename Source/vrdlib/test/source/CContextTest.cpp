
#include "../include/CReaderAwareTestBase.h"

#include "vrdlib/common/include/CContext.h"

#include <gtest/gtest.h>

#include <sstream>

using VRD::CContext;
using VRD::CDescriptor;

struct ContextTest : VRD::Test::CReaderAwareTestBase 
{
   ContextTest() : CReaderAwareTestBase("ContextTest", std::string()) {}
};

TEST_F(ContextTest, GetSetDescriptor)
{
   CContext context(getReader());
   context.setDescriptor(CDescriptor("nameA", VRD::Utility::Absolute(23), 2, 5, "descriptionA"));
   context.setDescriptor(CDescriptor("nameB", VRD::Utility::Absolute(5), 1, 23, "descriptionB"));
   EXPECT_EQ(context.getDescriptor("nameA").getName(), "nameA");
   EXPECT_EQ(context.getDescriptor("nameB").getName(), "nameB");
   EXPECT_THROW(context.getDescriptor("nameC").getName(), std::out_of_range);
}

TEST_F(ContextTest, ForEachDescriptor)
{
   CContext context(getReader());
   context.setDescriptor(CDescriptor("nameA", VRD::Utility::Absolute(23), 2, 5, "descriptionA"));
   context.setDescriptor(CDescriptor("nameB", VRD::Utility::Absolute(5), 1, 23, "descriptionB"));
   std::vector<CDescriptor> results;
   context.foreachDescriptor([&](CDescriptor const& d) { results.push_back(d); });
   EXPECT_EQ(results[0].getName(), "nameA");
   EXPECT_EQ(results[0].getPosition(), VRD::Utility::Absolute(23));
   EXPECT_EQ(results[0].getPayloadPosition(), VRD::Utility::Absolute(25));
   EXPECT_EQ(results[0].getEnd(), VRD::Utility::Absolute(30));
   EXPECT_EQ(results[1].getName(), "nameB");
   EXPECT_EQ(results[1].getPosition(), VRD::Utility::Absolute(5));
   EXPECT_EQ(results[1].getPayloadPosition(), VRD::Utility::Absolute(6));
   EXPECT_EQ(results[1].getEnd(), VRD::Utility::Absolute(29));
   EXPECT_EQ(results.size(), 2u);
}

TEST_F(ContextTest, ToString)
{
   CDescriptor d1("b", VRD::Utility::Absolute(23), 2, 5, "B");
   CDescriptor d2("a", VRD::Utility::Absolute(5), 1, 23, "A");
   CContext context(getReader());
   context.setDescriptor(d1);
   context.setDescriptor(d2);
   std::ostringstream os;
   os << context;
   EXPECT_EQ(os.str(), d2.toString() + '\n' + d1.toString());
}
