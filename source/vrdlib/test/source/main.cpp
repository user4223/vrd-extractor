
#include "vrdlib/utility/include/LoggerUtility.h"

#include <filesystem>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   ::testing::InitGoogleMock(&argc, argv);

   VRD::Utility::initializeTestLogger(std::filesystem::path(argv[0]));

   return RUN_ALL_TESTS();
}
