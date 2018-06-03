
#include "../include/CSampleDirectoryAwareTestBase.h"
#include "../include/CConflictHandlerMock.h"

#include "vrdlib/common/include/CExtractor.h"

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <memory>

using namespace testing;

struct ExtractorTest : public VRD::Test::CSampleAwareTestBase {};

TEST_F(ExtractorTest, Instantiation)
{
   auto conflictHandler(std::make_unique<VRD::Test::CConflictHandlerMock>());
   ON_CALL(*conflictHandler, handle(_)).WillByDefault(Return(0));
   
   EXPECT_NO_THROW(VRD::CExtractor(
       getSampleDirectory()
      ,".*[.]CR[2W]$"
      ,std::move(conflictHandler)
      ,true));
}
