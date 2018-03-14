
#include "../include/CSampleDirectoryAwareTestBase.h"

#include "vrdlib/common/include/CExtractor.h"

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

struct ExtractorTest : public VRD::Test::CSampleAwareTestBase {};

TEST_F(ExtractorTest, Instantiation)
{
   EXPECT_NO_THROW(VRD::CExtractor(getSampleDirectory(), ".*[.]CR[2W]$", true));
}
