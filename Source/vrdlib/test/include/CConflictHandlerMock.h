#pragma once

#include "vrdlib/api/include/IConflictHandler.h"

#include <gmock/gmock.h>

#include <functional>
#include <memory>

namespace VRD
{
namespace Test
{
   struct CConflictHandlerMock : API::IConflictHandler
   {
      MOCK_METHOD1(handle, Result(std::vector<std::string> const& options));
   };
   
   struct CConflictHandlerMockFactory : API::IConflictHandlerFactory
   {
      CConflictHandlerMockFactory(std::function<std::unique_ptr<API::IConflictHandler>()> creator)
         :m_creator(creator)
      {}
      
      virtual std::unique_ptr<API::IConflictHandler> create(boost::filesystem::path path) override
      {  return m_creator(); }
      
   private:
      std::function<std::unique_ptr<API::IConflictHandler>()> m_creator;
   };
}}
