
#include "../include/CPropertySourceMock.h"
#include "../include/CConflictHandlerMock.h"

#include "vrdlib/api/include/PropertyTypes.h"

#include "vrdlib/canon/include/CPropertyAdapter.h"
#include "vrdlib/canon/include/PropertyTypes.h"

#include <gtest/gtest.h>

using namespace VRD;
using namespace testing;

template <typename ValueT, typename PropertyT>
auto getProperty(PropertyT type, ValueT value)
{  return API::CProperty(to_string(type), API::CProperty::ValueType(value)); }

auto getRating(int16_t value)
{  return getProperty<int16_t>(API::PropertyType::Rating, value); }

auto getVRD1CheckMark(uint16_t value)
{  return getProperty<uint16_t>(Canon::PropertyType::VRD1_CheckMark, value); }

auto getVRD2CheckMark(uint16_t value)
{  return getProperty<uint16_t>(Canon::PropertyType::VRD2_CheckMark, value); }

auto getDR4CheckMark(uint32_t value)
{  return getProperty<uint32_t>(Canon::PropertyType::DR4_CheckMark, value); }

auto getXMPRating(int16_t value)
{  return getProperty<int16_t>(Canon::PropertyType::XMP_Rating, value); }

TEST(CanonPropertyAdapterTest, VRD1_CheckMark_Only)
{
   auto query(std::make_unique<VRD::Test::CPropertySourceMock>());
   EXPECT_CALL(*query, getProperty(_)).Times(AnyNumber());
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::VRD1_CheckMark)))
      .WillOnce(Return(getVRD1CheckMark(1)))
      .WillOnce(Return(getVRD1CheckMark(2)))
      .WillOnce(Return(getVRD1CheckMark(3)))
      .WillOnce(Return(getVRD1CheckMark(4))) ///< Values > 4 are unsupported in VRD1
      .WillOnce(Return(getVRD1CheckMark(0)));
   VRD::Test::CConflictHandlerMock conflictHandler;
   
   Canon::CPropertyAdapter adapter(std::move(query), conflictHandler);
   EXPECT_EQ(getRating(5), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(4), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(3), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating))); ///< 4 is unsupported, hence we get false here
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
}

TEST(CanonPropertyAdapterTest, VRD2_CheckMark_Only)
{
   auto query(std::make_unique<VRD::Test::CPropertySourceMock>());
   EXPECT_CALL(*query, getProperty(_)).Times(AnyNumber());
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::VRD2_CheckMark)))
      .WillOnce(Return(getVRD2CheckMark(1)))
      .WillOnce(Return(getVRD2CheckMark(2)))
      .WillOnce(Return(getVRD2CheckMark(3)))
      .WillOnce(Return(getVRD2CheckMark(4)))
      .WillOnce(Return(getVRD2CheckMark(5)))
      .WillOnce(Return(getVRD2CheckMark(6)))
      .WillOnce(Return(getVRD2CheckMark(0)));
   VRD::Test::CConflictHandlerMock conflictHandler;
   
   Canon::CPropertyAdapter adapter(std::move(query), conflictHandler);
   EXPECT_EQ(getRating(5), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(4), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(3), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(2), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(1), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
}

TEST(CanonPropertyAdapterTest, DR4_CheckMark_Only)
{
   auto query(std::make_unique<VRD::Test::CPropertySourceMock>());
   EXPECT_CALL(*query, getProperty(_)).Times(AnyNumber());
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::DR4_CheckMark)))
      .WillOnce(Return(getDR4CheckMark(1)))
      .WillOnce(Return(getDR4CheckMark(2)))
      .WillOnce(Return(getDR4CheckMark(3)))
      .WillOnce(Return(getDR4CheckMark(4)))
      .WillOnce(Return(getDR4CheckMark(5)))
      .WillOnce(Return(getDR4CheckMark(6)))
      .WillOnce(Return(getDR4CheckMark(0)));
   VRD::Test::CConflictHandlerMock conflictHandler;
   
   Canon::CPropertyAdapter adapter(std::move(query), conflictHandler);
   EXPECT_EQ(getRating(5), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(4), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(3), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(2), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(1), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
}

TEST(CanonPropertyAdapterTest, XMP_Rating_Only)
{
   auto query(std::make_unique<VRD::Test::CPropertySourceMock>());
   EXPECT_CALL(*query, getProperty(_)).Times(AnyNumber());
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::XMP_Rating)))
      .WillOnce(Return(getXMPRating(1)))
      .WillOnce(Return(getXMPRating(2)))
      .WillOnce(Return(getXMPRating(3)))
      .WillOnce(Return(getXMPRating(4)))
      .WillOnce(Return(getXMPRating(5)))
      .WillOnce(Return(getXMPRating(6)))
      .WillOnce(Return(getXMPRating(0)))
      .WillOnce(Return(getXMPRating(-1)))
      .WillOnce(Return(getXMPRating(-2)));
   VRD::Test::CConflictHandlerMock conflictHandler;
   
   Canon::CPropertyAdapter adapter(std::move(query), conflictHandler);
   EXPECT_EQ(getRating(1), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(2), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(3), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(4), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(5), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(-1), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
}

TEST(CanonPropertyAdapterTest, UnmatchingConflictSelection)
{
   auto query(std::make_unique<VRD::Test::CPropertySourceMock>());
   EXPECT_CALL(*query, getProperty(_)).Times(AnyNumber());
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::VRD1_CheckMark)))
      .WillOnce(Return(getVRD1CheckMark(2)));
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::VRD2_CheckMark)))
      .WillOnce(Return(getVRD2CheckMark(1)));
   VRD::Test::CConflictHandlerMock conflictHandler;
   EXPECT_CALL(conflictHandler, handle(_)).WillOnce(Return(3));

   Canon::CPropertyAdapter adapter(std::move(query), conflictHandler);

   EXPECT_THROW(adapter.getProperty(to_string(API::PropertyType::Rating)), std::domain_error);
}

TEST(CanonPropertyAdapterTest, Conflicting_VRD1_VRD2_CheckMark)
{
   auto query(std::make_unique<VRD::Test::CPropertySourceMock>());
   EXPECT_CALL(*query, getProperty(_)).Times(AnyNumber());
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::VRD1_CheckMark)))
      .WillOnce(Return(getVRD1CheckMark(2)))
      .WillOnce(Return(getVRD1CheckMark(3)))
      .WillOnce(Return(getVRD1CheckMark(1)))
      .WillOnce(Return(getVRD1CheckMark(4))) ///< Values > 4 are unsupported in VRD1, so this is not a conflict
      .WillRepeatedly(Return(getVRD1CheckMark(0)));
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::VRD2_CheckMark)))
      .WillOnce(Return(getVRD2CheckMark(1)))
      .WillOnce(Return(getVRD2CheckMark(2)))
      .WillOnce(Return(getVRD2CheckMark(2)))
      .WillOnce(Return(getVRD2CheckMark(4)))
      .WillOnce(Return(getVRD2CheckMark(5)))
      .WillRepeatedly(Return(getVRD2CheckMark(0)));
   VRD::Test::CConflictHandlerMock conflictHandler;
   EXPECT_CALL(conflictHandler, handle(_))
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1));
   
   Canon::CPropertyAdapter adapter(std::move(query), conflictHandler);

   EXPECT_EQ(getRating(4), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(3), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(5), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(2), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(1), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
}

TEST(CanonPropertyAdapterTest, Conflicting_VRD2_DR4_CheckMark)
{
   auto query(std::make_unique<VRD::Test::CPropertySourceMock>());
   EXPECT_CALL(*query, getProperty(_)).Times(AnyNumber());
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::VRD2_CheckMark)))
      .WillOnce(Return(getVRD2CheckMark(1)))
      .WillOnce(Return(getVRD2CheckMark(4)))
      .WillOnce(Return(getVRD2CheckMark(0))) ///< 0 does not produce a conflict
      .WillOnce(Return(getVRD2CheckMark(4)))
      .WillRepeatedly(Return(getVRD2CheckMark(0)));
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::DR4_CheckMark)))
      .WillOnce(Return(getDR4CheckMark(4)))
      .WillOnce(Return(getDR4CheckMark(2)))
      .WillOnce(Return(getDR4CheckMark(4)))
      .WillOnce(Return(getDR4CheckMark(5)))
      .WillRepeatedly(Return(getDR4CheckMark(0)));
   VRD::Test::CConflictHandlerMock conflictHandler;
   EXPECT_CALL(conflictHandler, handle(_))
      .WillOnce(Return(1))
      .WillOnce(Return(0))
      .WillOnce(Return(0));
   
   Canon::CPropertyAdapter adapter(std::move(query), conflictHandler);
 
   EXPECT_EQ(getRating(5), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(2), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(2), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(1), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
}

TEST(CanonPropertyAdapterTest, Conflicting_DR4_CheckMark_XMP_Rating)
{
   auto query(std::make_unique<VRD::Test::CPropertySourceMock>());
   EXPECT_CALL(*query, getProperty(_)).Times(AnyNumber());
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::DR4_CheckMark)))
      .WillOnce(Return(getDR4CheckMark(4)))
      .WillOnce(Return(getDR4CheckMark(1)))
      .WillOnce(Return(getDR4CheckMark(2)))
      .WillRepeatedly(Return(getDR4CheckMark(0)));
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::XMP_Rating)))
      .WillOnce(Return(getXMPRating(5)))
      .WillOnce(Return(getXMPRating(-1)))
      .WillOnce(Return(getXMPRating(5)))
      .WillOnce(Return(getXMPRating(4)))
      .WillOnce(Return(getXMPRating(-1)))
      .WillRepeatedly(Return(getXMPRating(0)));
   VRD::Test::CConflictHandlerMock conflictHandler;
   EXPECT_CALL(conflictHandler, handle(_))
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(0));

   Canon::CPropertyAdapter adapter(std::move(query), conflictHandler);
   
   EXPECT_EQ(getRating(5), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(5), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(4), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(4), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(-1), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
}

TEST(CanonPropertyAdapterTest, Conflicting_VRD2_DR4_CheckMark_XMP_Rating)
{
   auto query(std::make_unique<VRD::Test::CPropertySourceMock>());
   EXPECT_CALL(*query, getProperty(_)).Times(AnyNumber());
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::VRD2_CheckMark)))
      .WillOnce(Return(getVRD2CheckMark(3))) // 3
      .WillOnce(Return(getVRD2CheckMark(2))) // 4
      .WillOnce(Return(getVRD2CheckMark(1))) // 5
      .WillOnce(Return(getVRD2CheckMark(4))) // 2
      .WillRepeatedly(Return(getVRD2CheckMark(0)));
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::DR4_CheckMark)))
      .WillOnce(Return(getDR4CheckMark(4))) // 2
      .WillOnce(Return(getDR4CheckMark(5))) // 1
      .WillOnce(Return(getDR4CheckMark(2))) // 4
      .WillRepeatedly(Return(getDR4CheckMark(0)));
   EXPECT_CALL(*query, getProperty(to_string(Canon::PropertyType::XMP_Rating)))
      .WillOnce(Return(getXMPRating(5))) // 5
      .WillOnce(Return(getXMPRating(-1)))
      .WillOnce(Return(getXMPRating(4))) // 4
      .WillOnce(Return(getXMPRating(3))) // 3
      .WillOnce(Return(getXMPRating(-1)))
      .WillRepeatedly(Return(getXMPRating(0)));
   VRD::Test::CConflictHandlerMock conflictHandler;
   EXPECT_CALL(conflictHandler, handle(_))
      .WillOnce(Return(2))
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1));
   
   Canon::CPropertyAdapter adapter(std::move(query), conflictHandler);
   
   EXPECT_EQ(getRating(5), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(-1), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(4), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(3), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_EQ(getRating(-1), *adapter.getProperty(to_string(API::PropertyType::Rating)));
   EXPECT_FALSE(adapter.getProperty(to_string(API::PropertyType::Rating)));
}
