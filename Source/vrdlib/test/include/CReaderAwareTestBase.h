#pragma once

#include "CTempDirectoryAwareTestBase.h"

#include "vrdlib/utility/include/CStreamReader.h"

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

namespace VRD
{
namespace Test
{
   class CReaderAwareTestBase : public CTempDirectoryAwareTestBase
   {
   protected:
      CReaderAwareTestBase(std::string pathPostfix, std::string buffer)
         :CTempDirectoryAwareTestBase(pathPostfix)
         ,m_buffer(std::move(buffer))
         ,m_uniqueFilePath(getDirectoryPath() / "binary_file")
         ,m_reader() 
      {}

      virtual void SetUp() override
      { 
         CTempDirectoryAwareTestBase::SetUp();
         std::ofstream stream(m_uniqueFilePath.string());
         stream.write(&m_buffer[0], m_buffer.size());
      }
      
      boost::filesystem::path getFilePath() const
      {  return m_uniqueFilePath; }
      
      VRD::Utility::CStreamReader& getReader() 
      { 
         if (!m_reader)
         {  m_reader = std::make_unique<VRD::Utility::CStreamReader>(m_uniqueFilePath.string()); }
         return *m_reader; 
      }
      
   private:
      std::string m_buffer;
      boost::filesystem::path m_uniqueFilePath;
      std::unique_ptr<VRD::Utility::CStreamReader> m_reader;
   };
}}
