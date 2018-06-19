
#include "vrdlib/api/include/CProperty.h"

#include "vrdlib/common/include/CPropertyHandler.h"

#include <gtest/gtest.h>

using VRD::Common::CPropertyHandler;
using VRD::API::CProperty;

TEST(PropertyHandlerTest, SetGetProperty)
{
   CPropertyHandler handler;
   EXPECT_NO_THROW(handler.setProperty(CProperty("bla", CProperty::ValueType(1), "blub")));
   EXPECT_EQ(handler.getProperty("bla").value(), CProperty("bla", CProperty::ValueType(1), "blub"));
}

TEST(PropertyHandlerTest, ForeachProperty)
{
   CPropertyHandler handler;
   EXPECT_NO_THROW(handler.setProperty(CProperty("bli", CProperty::ValueType(23), "blubblub")));
   EXPECT_NO_THROW(handler.setProperty(CProperty("bla", CProperty::ValueType(5), "blub")));   
   std::vector<CProperty> results;
   EXPECT_EQ(std::make_pair(2u, 2u), handler.foreachProperty([&](CProperty const& p) { results.push_back(p); return true; }));
   EXPECT_EQ(results[0].name, "bla");
   EXPECT_EQ(results[0].value, CProperty::ValueType(5));
   EXPECT_EQ(results[0].description, "blub");
   EXPECT_EQ(results[1].name, "bli");
   EXPECT_EQ(results[1].value, CProperty::ValueType(23));
   EXPECT_EQ(results[1].description, "blubblub");
   EXPECT_EQ(results.size(), 2u);
}

TEST(PropertyHandlerTest, ForeachPropertyUnsuccessful)
{
   CPropertyHandler handler;
   EXPECT_NO_THROW(handler.setProperty(CProperty("bli", CProperty::ValueType(23), "blubblub")));
   EXPECT_NO_THROW(handler.setProperty(CProperty("bla", CProperty::ValueType(5), "blub")));   
   EXPECT_EQ(std::make_pair(2u, 0u), handler.foreachProperty([&](CProperty const& p) { return false; }));
}

TEST(PropertyHandlerTest, ToString)
{
   CProperty const p1("bli", CProperty::ValueType(23), "description");
   CProperty const p2("bla", CProperty::ValueType(5), "descr");
   CPropertyHandler handler;
   EXPECT_NO_THROW(handler.setProperty(p1));
   EXPECT_NO_THROW(handler.setProperty(p2));
   EXPECT_EQ(handler.toString(), to_string(p2) + "\n" + to_string(p1));
}
