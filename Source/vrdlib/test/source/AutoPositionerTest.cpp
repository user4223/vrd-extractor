
#include "vrdlib/common/include/CAutoPositioner.h"

#include "vrdlib/test/include/CReaderAwareTestBase.h"

#include <gtest/gtest.h>

struct AutoPositionerTest : VRD::Test::CReaderAwareTestBase 
{
   AutoPositionerTest() : CReaderAwareTestBase("AutoPositionerTest", std::string({
       0,0,0,1
      ,0,0,0,2
      ,0,3
      ,0,0,0,4
      ,0,0,0,5
      ,0,0,0,6
      ,0,0,0,7
      ,0,0,0,8
   })) {}
};

TEST_F(AutoPositionerTest, StartEnd)
{
   auto& reader(getReader());
   EXPECT_EQ(VRD::Utility::Absolute(0), reader.getPosition());
   EXPECT_EQ(1u, reader.readAt<uint32_t>(VRD::Utility::Absolute(0)));
   EXPECT_EQ(VRD::Utility::Absolute(0), reader.getPosition());
   {
      VRD::CAutoPositioner position(reader, VRD::CDescriptor("bla", VRD::Utility::Absolute(8), 2, 12));
      EXPECT_EQ(VRD::Utility::Absolute(10), reader.getPosition());
      EXPECT_EQ(4u, reader.readAt<uint32_t>(VRD::Utility::Relative(0)));
   }
   EXPECT_EQ(VRD::Utility::Absolute(22), reader.getPosition());   
   EXPECT_EQ(7u, reader.readAt<uint32_t>(VRD::Utility::Relative(0)));   
}

TEST_F(AutoPositionerTest, VariableSize)
{
   auto& reader(getReader());
   EXPECT_EQ(VRD::Utility::Absolute(0), reader.getPosition());
   {
      VRD::CAutoPositioner position(reader, VRD::CDescriptor("bla", VRD::Utility::Absolute(8), 2, 12));
      EXPECT_EQ(VRD::Utility::Absolute(10), reader.getPosition());
      position.addSize(4);
   }
   EXPECT_EQ(VRD::Utility::Absolute(26), reader.getPosition());   
   EXPECT_EQ(8u, reader.readAt<uint32_t>(VRD::Utility::Relative(0)));   
}
