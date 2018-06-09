
#include "vrdlib/utility/include/PositionUtility.h"

#include <gtest/gtest.h>

TEST(AbsoluteTest, AddPositive)
{
   EXPECT_EQ(std::streampos(23), VRD::Utility::Absolute(5) + VRD::Utility::Relative(18));
}

TEST(AbsoluteTest, AddNegative)
{
   EXPECT_EQ(std::streampos(5), VRD::Utility::Absolute(23) + VRD::Utility::Relative(-18));
}

TEST(AbsoluteTest, GoingNegative)
{
   EXPECT_THROW(VRD::Utility::Absolute(5) + VRD::Utility::Relative(-7), std::range_error);
}

TEST(AbsoluteTest, CreateNegative)
{
   EXPECT_NO_THROW(VRD::Utility::Absolute(-5)); ///< To move from end
}

TEST(AbsoluteTest, CreatePositive)
{
   EXPECT_NO_THROW(VRD::Utility::Absolute(23)); ///< To move from begin
}

TEST(AbsoluteTest, Equal)
{
   EXPECT_EQ(VRD::Utility::Absolute(23), VRD::Utility::Absolute(23));
}

TEST(AbsoluteTest, NotEqual)
{
   EXPECT_NE(VRD::Utility::Absolute(23), VRD::Utility::Absolute(5));
}

TEST(AbsoluteTest, GreaterThan)
{
   EXPECT_TRUE(VRD::Utility::Absolute(4) < VRD::Utility::Absolute(5));
   EXPECT_FALSE(VRD::Utility::Absolute(6) < VRD::Utility::Absolute(5));
}

TEST(AbsoluteTest, LessThan)
{
   EXPECT_TRUE(VRD::Utility::Absolute(6) > VRD::Utility::Absolute(5));
   EXPECT_FALSE(VRD::Utility::Absolute(4) > VRD::Utility::Absolute(5));
}

TEST(AbsoluteTest, ToString)
{
   EXPECT_EQ("0x00000004 (00000004)", to_string(VRD::Utility::Absolute(4)));
   std::ostringstream os;
   os << VRD::Utility::Absolute(23);
   EXPECT_EQ("0x00000017 (00000023)", os.str());
}
