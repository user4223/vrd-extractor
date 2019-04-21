
#include "vrdlib/common/include/CDescriptor.h"

#include <gtest/gtest.h>

TEST(DescriptorTest, Parameters)
{
   VRD::CDescriptor descriptor("name of descriptor", VRD::Utility::Absolute(23), 2, 5, "description of descriptor");
   EXPECT_EQ("name of descriptor", descriptor.getName());
   EXPECT_EQ(VRD::Utility::Absolute(23), descriptor.getPosition());
   EXPECT_EQ(VRD::Utility::Absolute(25), descriptor.getPayloadPosition());
   EXPECT_EQ(VRD::Utility::Absolute(30), descriptor.getEnd());
   EXPECT_EQ(7u, descriptor.getSize());   
   EXPECT_EQ(5u, descriptor.getPayloadSize());
   EXPECT_EQ("description of descriptor", descriptor.getDescription());
}
