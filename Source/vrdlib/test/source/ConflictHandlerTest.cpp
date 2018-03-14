#include "vrdlib/utility/include/CConflictHandler.h"

#include <gtest/gtest.h>

using namespace VRD;

std::stringstream makeInput(std::vector<int> values)
{
   std::stringstream s;
   for (auto v : values)
   {  s << v << '\n'; }
   return s;
}

TEST(CManualConflictHandler, SelectInRange)
{
   auto stream(makeInput({1,0,2}));
   std::vector<std::string> options({"OptionA","OptionB","OptionC"});
   
   Utility::CManualConflictHandler h(stream, std::cout);
   EXPECT_EQ(API::IConflictHandler::Result(1), h.handle(options));
   EXPECT_EQ(API::IConflictHandler::Result(0), h.handle(options));
   EXPECT_EQ(API::IConflictHandler::Result(2), h.handle(options));
}

TEST(CManualConflictHandler, EmptyOptionList)
{
   auto stream(makeInput({1}));
   std::vector<std::string> options;
   
   Utility::CManualConflictHandler h(stream, std::cout);
   EXPECT_THROW(h.handle(options), std::invalid_argument);
}

TEST(CManualConflictHandler, SelectAboveRange)
{
   auto stream(makeInput({2,0}));   
   std::vector<std::string> options({"OptionA","OptionB"});
   
   VRD::Utility::CManualConflictHandler h(stream, std::cout);
   EXPECT_EQ(API::IConflictHandler::Result(0), h.handle(options));
}

TEST(CManualConflictHandler, SelectBelowRange)
{
   auto stream(makeInput({-1,1}));
   std::vector<std::string> options({"OptionA","OptionB"});
   
   VRD::Utility::CManualConflictHandler h(stream, std::cout);
   EXPECT_EQ(API::IConflictHandler::Result(1), h.handle(options));
}

TEST(CManualConflictHandler, InvalidSelection)
{
   std::stringstream s;
   s << "blub";
   
   std::vector<std::string> options({"OptionA","OptionB"});
   
   VRD::Utility::CManualConflictHandler h(s, std::cout);
   EXPECT_THROW(h.handle(options), std::domain_error);
}

TEST(CManualConflictHandler, Terminate)
{
   std::stringstream s;
   s << "-2\nX";
   
   std::vector<std::string> options({"OptionA","OptionB"});
   
   VRD::Utility::CManualConflictHandler h(s, std::cout);
   EXPECT_THROW(h.handle(options), std::domain_error);
}

TEST(CManualConflictHandler, EmptyInput)
{
   std::stringstream s;
   s << "\n\n\n"; ///< 3 tries, then EOF
   
   std::vector<std::string> options({"OptionA"});
   
   VRD::Utility::CManualConflictHandler h(s, std::cout);
   EXPECT_THROW(h.handle(options), std::domain_error);
}
