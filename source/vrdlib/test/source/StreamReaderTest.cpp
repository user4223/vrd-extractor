
#include "vrdlib/utility/include/CStreamReader.h"

#include "../include/CReaderAwareTestBase.h"

#include <gtest/gtest.h>

struct StreamReaderTest : VRD::Test::CReaderAwareTestBase 
{
   StreamReaderTest() 
      :CReaderAwareTestBase("StreamReaderTest", std::string({
          0,0,0,1
         ,1,0,0,0
         ,0,2
         ,2,0
         ,'h','e','l','l','o',0
         ,-1,-1,-1,-23
         ,64,55,-128,0,0,0,0,0
      }))
   {}
};

TEST_F(StreamReaderTest, OpenDirectory)
{
   EXPECT_THROW(VRD::Utility::CStreamReader(getDirectoryPath(), VRD::Utility::ByteOrder::LittleEndian), std::invalid_argument);
}

TEST_F(StreamReaderTest, NotExistingFile)
{
   EXPECT_THROW(VRD::Utility::CStreamReader(getDirectoryPath() / "blub", VRD::Utility::ByteOrder::LittleEndian), std::invalid_argument);
}

TEST_F(StreamReaderTest, Empty)
{
   VRD::Utility::CStreamReader reader(getFilePath(), VRD::Utility::ByteOrder::LittleEndian);
   EXPECT_EQ(std::vector<uint8_t>(), reader.read(0));
}

TEST_F(StreamReaderTest, Endianess)
{
   VRD::Utility::CStreamReader reader(getFilePath(), VRD::Utility::ByteOrder::LittleEndian);
   EXPECT_EQ(1u,        reader.readAt<uint32_t>(VRD::Utility::Relative(0)));
   EXPECT_EQ(16777216u, reader.readAt<uint32_t>(VRD::Utility::Relative(4)));
   EXPECT_EQ(2u,        reader.readAt<uint16_t>(VRD::Utility::Relative(8)));
   EXPECT_EQ(512u,      reader.readAt<uint16_t>(VRD::Utility::Relative(10)));
   EXPECT_EQ("hello",   reader.readAt<std::string>(12));
   EXPECT_EQ(-23,       reader.readAt<int32_t>(VRD::Utility::Relative(18)));
   reader.setByteOrder(VRD::Utility::ByteOrder::BigEndian);
   EXPECT_EQ(16777216u, reader.readAt<uint32_t>(VRD::Utility::Relative(0)));
   EXPECT_EQ(1u,        reader.readAt<uint32_t>(VRD::Utility::Relative(4)));
   EXPECT_EQ(512u,      reader.readAt<uint16_t>(VRD::Utility::Relative(8)));
   EXPECT_EQ(2u,        reader.readAt<uint16_t>(VRD::Utility::Relative(10)));
   EXPECT_EQ("hello",   reader.readAt<std::string>(12));
   EXPECT_EQ(-369098753,reader.readAt<int32_t>(VRD::Utility::Relative(18)));
}

TEST_F(StreamReaderTest, AutoEndianess)
{  
   VRD::Utility::CStreamReader reader(getFilePath(), VRD::Utility::ByteOrder::BigEndian);
   EXPECT_EQ(16777216u, reader.readAt<uint32_t>(VRD::Utility::Relative(0)));
   EXPECT_EQ(1u,        reader.readAt<uint32_t>(VRD::Utility::Relative(4)));
   {
      auto const token1(reader.setByteOrderTemporarily(VRD::Utility::ByteOrder::LittleEndian));
      EXPECT_EQ(1u,        reader.readAt<uint32_t>(VRD::Utility::Relative(0)));
      EXPECT_EQ(16777216u, reader.readAt<uint32_t>(VRD::Utility::Relative(4)));
      {
         auto const token2(reader.setByteOrderTemporarily(VRD::Utility::ByteOrder::BigEndian));
         EXPECT_EQ(16777216u, reader.readAt<uint32_t>(VRD::Utility::Relative(0)));
         EXPECT_EQ(1u,        reader.readAt<uint32_t>(VRD::Utility::Relative(4)));
      }
      EXPECT_EQ(1u,        reader.readAt<uint32_t>(VRD::Utility::Relative(0)));
      EXPECT_EQ(16777216u, reader.readAt<uint32_t>(VRD::Utility::Relative(4)));
   }
   EXPECT_EQ(16777216u, reader.readAt<uint32_t>(VRD::Utility::Relative(0)));
   EXPECT_EQ(1u,        reader.readAt<uint32_t>(VRD::Utility::Relative(4)));
}

TEST_F(StreamReaderTest, Position)
{
   auto& reader(getReader());
   EXPECT_EQ(VRD::Utility::Absolute(0), reader.getPosition());
   EXPECT_EQ(1u, reader.read<uint32_t>());
   EXPECT_EQ(VRD::Utility::Absolute(4), reader.getPosition());
   reader.moveTo(VRD::Utility::Relative(16));
   EXPECT_EQ(VRD::Utility::Absolute(20), reader.getPosition());
   reader.moveTo(VRD::Utility::Relative(-20));
   EXPECT_EQ(VRD::Utility::Absolute(0), reader.getPosition());
}

TEST_F(StreamReaderTest, ReadContinuously)
{
   auto& reader(getReader());
   EXPECT_EQ(1u,        reader.read<uint32_t>());
   EXPECT_EQ(16777216u, reader.read<uint32_t>());
   EXPECT_EQ(2u,        reader.read<uint16_t>());
   EXPECT_EQ(512u,      reader.read<uint16_t>());
   EXPECT_EQ("hello",   reader.read<std::string>());
   EXPECT_EQ(-1,        reader.read<int16_t>());
   EXPECT_EQ(-23,       reader.read<int16_t>());   
}

TEST_F(StreamReaderTest, ReadRelative)
{
   auto& reader(getReader());
   reader.moveTo(VRD::Utility::Absolute(8));
   EXPECT_EQ(2u,  reader.readAt<uint16_t>(VRD::Utility::Relative(0)));
   EXPECT_EQ(1u,  reader.readAt<uint32_t>(VRD::Utility::Relative(-8)));
   EXPECT_EQ(-23, reader.readAt<int32_t>(VRD::Utility::Relative(10)));  
}

TEST_F(StreamReaderTest, ReadAbsolute)
{
   auto& reader(getReader());
   reader.moveTo(VRD::Utility::Absolute(8));
   EXPECT_EQ(2u,  reader.read<uint16_t>()); ///< To check
   EXPECT_EQ(1u,  reader.readAt<uint32_t>(VRD::Utility::Absolute(0)));
   EXPECT_EQ(-23, reader.readAt<int32_t>(VRD::Utility::Absolute(18)));
   EXPECT_EQ(-23, reader.readAt<int32_t>(VRD::Utility::Absolute(-12)));   ///< From end
   EXPECT_EQ(1u,  reader.readAt<uint32_t>(VRD::Utility::Absolute(-30))); ///< From end
}


TEST_F(StreamReaderTest, ReadBeyondEnd)
{
   auto& reader(getReader());
   reader.moveTo(VRD::Utility::Absolute(8));
   EXPECT_THROW(reader.readAt<uint32_t>(VRD::Utility::Relative(128)), std::range_error);
}

TEST_F(StreamReaderTest, ReadBeforeBegin)
{
   auto& reader(getReader());
   reader.moveTo(VRD::Utility::Absolute(8));
   EXPECT_THROW(reader.readAt<uint32_t>(VRD::Utility::Relative(-128)), std::range_error);
}

TEST_F(StreamReaderTest, ReadInt)
{
   auto& reader(getReader());
   EXPECT_EQ(-23, reader.readAt<int32_t>(VRD::Utility::Relative(18)));
   EXPECT_EQ(-23, reader.readAt<int16_t>(VRD::Utility::Relative(20)));
   EXPECT_EQ(-23, reader.readAt<int8_t>(VRD::Utility::Relative(21)));
   reader.moveTo(VRD::Utility::Absolute(18));
   EXPECT_EQ(-23, reader.read<int32_t>());
   reader.moveTo(VRD::Utility::Relative(-2));
   EXPECT_EQ(-23, reader.read<int16_t>());
   reader.moveTo(VRD::Utility::Relative(-1));
   EXPECT_EQ(-23, reader.read<int8_t>());
}

TEST_F(StreamReaderTest, ReadUnsignedInt)
{
   auto& reader(getReader());
   EXPECT_EQ(2u, reader.readAt<uint32_t>(VRD::Utility::Relative(6)));
   EXPECT_EQ(2u, reader.readAt<uint16_t>(VRD::Utility::Relative(8)));
   EXPECT_EQ(2u, reader.readAt<uint8_t>(VRD::Utility::Relative(9)));
   reader.moveTo(VRD::Utility::Absolute(6));
   EXPECT_EQ(2u, reader.read<uint32_t>());
   reader.moveTo(VRD::Utility::Relative(-2));
   EXPECT_EQ(2u, reader.read<uint16_t>());
   reader.moveTo(VRD::Utility::Relative(-1));
   EXPECT_EQ(2u, reader.read<uint8_t>());
}

TEST_F(StreamReaderTest, ReadString)
{
   auto& reader(getReader());
   reader.moveTo(VRD::Utility::Absolute(12));
   EXPECT_EQ(std::vector<uint8_t>({'h','e','l','l','o', 0}), reader.read(6));
   reader.moveTo(VRD::Utility::Relative(-6));
   EXPECT_EQ("hello", reader.read<std::string>());
   reader.moveTo(VRD::Utility::Relative(-6));
   EXPECT_EQ("hello", reader.readRaw<std::string>());
   EXPECT_EQ("hello", reader.readAt<std::string>(VRD::Utility::Relative(-6)));
   EXPECT_EQ("hello", reader.readAt<std::string>(VRD::Utility::Absolute(12)));
}

TEST_F(StreamReaderTest, ReadDouble)
{
   auto& reader(getReader());
   reader.moveTo(VRD::Utility::Absolute(22));
   EXPECT_EQ(23.5, reader.read<double>());
}

TEST_F(StreamReaderTest, Output)
{
   auto& reader(getReader());
   reader.moveTo(VRD::Utility::Absolute(23));
   std::ostringstream os;
   os << reader;
   EXPECT_EQ(std::string("Position: ") + to_string(VRD::Utility::Absolute(23)), os.str());
}

TEST(ByteOrderTest, Output)
{
   {
      std::ostringstream os;
      os << VRD::Utility::ByteOrder::BigEndian;
      EXPECT_EQ("big endian", os.str());
   }
   {
      std::ostringstream os;
      os << VRD::Utility::ByteOrder::LittleEndian;
      EXPECT_EQ("little endian", os.str());
   }
}
