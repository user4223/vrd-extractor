
#include "vrdlib/api/include/PropertyTypes.h"
#include "vrdlib/common/include/CXMPWriter.h"

#include "../include/CTempDirectoryAwareTestBase.h"
#include "../include/CConflictHandlerMock.h"

#include <gtest/gtest.h>

#include <exiv2/exiv2.hpp>

#include <exception>
#include <chrono>

using namespace VRD;
using namespace testing;

struct XMPWriterTest : VRD::Test::CTempDirectoryAwareTestBase
{
   XMPWriterTest() : CTempDirectoryAwareTestBase("XMPWriterTest") {}

   Exiv2::Xmpdatum getEntry(std::filesystem::path file, std::string keyString)
   {
      auto image(Exiv2::ImageFactory::open((getDirectoryPath() / file).string()));
      image->readMetadata();
      auto const xmp(image->xmpData());
      Exiv2::XmpKey const key(keyString);
      auto const entry(xmp.findKey(key));
      return entry == xmp.end() ? Exiv2::Xmpdatum(key) : *entry;
   }
};

TEST_F(XMPWriterTest, WriteProperty)
{
   CXMPWriter writer(getDirectoryPath() / "WriteProperty.xmp", std::make_unique<VRD::Test::CConflictHandlerMock>(), CXMPWriter::Mode::Normal);
   EXPECT_FALSE(writer.hasChanged());
   EXPECT_TRUE(writer.setProperty(API::CProperty(to_string(API::PropertyType::Rating), API::CProperty::ValueType(2))));
   EXPECT_TRUE(writer.hasChanged());
   EXPECT_NO_THROW(writer.write());
   EXPECT_FALSE(writer.hasChanged());
   EXPECT_EQ(getEntry("WriteProperty.xmp", "Xmp.xmp.Rating").toUint32(), 2);
}

TEST_F(XMPWriterTest, OverwriteExisting)
{
   {
      CXMPWriter writer(getDirectoryPath() / "OverwriteExisting.xmp", std::make_unique<VRD::Test::CConflictHandlerMock>(), CXMPWriter::Mode::Normal);
      EXPECT_TRUE(writer.setProperty(API::CProperty(to_string(API::PropertyType::Rating), API::CProperty::ValueType(4))));
      EXPECT_NO_THROW(writer.write());
   }
   EXPECT_EQ(getEntry("OverwriteExisting.xmp", "Xmp.xmp.Rating").toUint32(), 4);
   {
      auto conflictHandler(std::make_unique<VRD::Test::CConflictHandlerMock>());
      EXPECT_CALL(*conflictHandler, handle(_)).WillOnce(Return(1));
      CXMPWriter writer(getDirectoryPath() / "OverwriteExisting.xmp", std::move(conflictHandler), CXMPWriter::Mode::Normal);
      EXPECT_TRUE(writer.setProperty(API::CProperty(to_string(API::PropertyType::Rating), API::CProperty::ValueType(3)))); ///< Change value
      EXPECT_NO_THROW(writer.write());
   }
   EXPECT_EQ(getEntry("OverwriteExisting.xmp", "Xmp.xmp.Rating").toUint32(), 3);
}

TEST_F(XMPWriterTest, DryMode)
{
   CXMPWriter writer(getDirectoryPath() / "DryMode.xmp", std::make_unique<VRD::Test::CConflictHandlerMock>(), CXMPWriter::Mode::Dry);
   EXPECT_FALSE(writer.hasChanged());
   EXPECT_TRUE(writer.setProperty(API::CProperty(to_string(API::PropertyType::Rating), API::CProperty::ValueType(2))));
   EXPECT_TRUE(writer.hasChanged());
   EXPECT_NO_THROW(writer.write());
   EXPECT_FALSE(writer.hasChanged());
   EXPECT_FALSE(std::filesystem::exists(getDirectoryPath() / "DryMode.xmp"));
}

TEST_F(XMPWriterTest, NoDataNoWrite)
{
   CXMPWriter writer(getDirectoryPath() / "NoDataNoWrite.xmp", std::make_unique<VRD::Test::CConflictHandlerMock>(), CXMPWriter::Mode::Normal);
   EXPECT_FALSE(writer.hasChanged());
   EXPECT_NO_THROW(writer.write());
   EXPECT_FALSE(std::filesystem::exists(getDirectoryPath() / "NoDataNoWrite.xmp"));
}

TEST_F(XMPWriterTest, NoChangeNoWrite)
{
   auto const path(getDirectoryPath() / "NoChangeNoWrite.xmp");
   auto const expectedTimePoint(std::chrono::file_clock::now() - std::chrono::hours(1));

   {
      CXMPWriter writer(path, std::make_unique<VRD::Test::CConflictHandlerMock>(), CXMPWriter::Mode::Normal);
      EXPECT_TRUE(writer.setProperty(API::CProperty(to_string(API::PropertyType::Rating), API::CProperty::ValueType(4))));
      EXPECT_NO_THROW(writer.write());
   }
   EXPECT_EQ(getEntry("NoChangeNoWrite.xmp", "Xmp.xmp.Rating").toUint32(), 4);

   std::filesystem::last_write_time(path, expectedTimePoint);        ///< Set file time
   auto const realTimePoint(std::filesystem::last_write_time(path)); ///< Re-read real file time
   {
      CXMPWriter writer(path, std::make_unique<VRD::Test::CConflictHandlerMock>(), CXMPWriter::Mode::Normal);
      EXPECT_FALSE(writer.setProperty(API::CProperty(to_string(API::PropertyType::Rating), API::CProperty::ValueType(4)))); ///< Same value
      EXPECT_NO_THROW(writer.write());
   }
   EXPECT_EQ(std::filesystem::last_write_time(path), realTimePoint); ///< No change
}

TEST_F(XMPWriterTest, ConflictUpdate)
{
   auto conflictHandler(std::make_unique<VRD::Test::CConflictHandlerMock>());
   EXPECT_CALL(*conflictHandler, handle(_)).WillOnce(Return(1));
   CXMPWriter writer(getDirectoryPath() / "ConflictUpdate.xmp", std::move(conflictHandler), CXMPWriter::Mode::Normal);
   EXPECT_TRUE(writer.setProperty(API::CProperty(to_string(API::PropertyType::Rating), API::CProperty::ValueType(2))));
   EXPECT_TRUE(writer.setProperty(API::CProperty(to_string(API::PropertyType::Rating), API::CProperty::ValueType(3))));
   EXPECT_NO_THROW(writer.write());
   EXPECT_EQ(getEntry("ConflictUpdate.xmp", "Xmp.xmp.Rating").toUint32(), 3);
}

TEST_F(XMPWriterTest, ConflictKeep)
{
   auto conflictHandler(std::make_unique<VRD::Test::CConflictHandlerMock>());
   EXPECT_CALL(*conflictHandler, handle(_)).WillOnce(Return(0));
   CXMPWriter writer(getDirectoryPath() / "ConflictKeep.xmp", std::move(conflictHandler), CXMPWriter::Mode::Normal);
   EXPECT_TRUE(writer.setProperty(API::CProperty(to_string(API::PropertyType::Rating), API::CProperty::ValueType(1))));
   EXPECT_FALSE(writer.setProperty(API::CProperty(to_string(API::PropertyType::Rating), API::CProperty::ValueType(3))));
   EXPECT_NO_THROW(writer.write());
   EXPECT_EQ(getEntry("ConflictKeep.xmp", "Xmp.xmp.Rating").toUint32(), 1);
}
