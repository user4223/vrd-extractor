#pragma once

#include "vrdlib/api/include/IConflictHandler.h"

#include <gmock/gmock.h>

namespace VRD
{
namespace Test
{
   struct CConflictHandlerMock : API::IConflictHandler
   {
      MOCK_METHOD1(handle, Result(std::vector<std::string> const& options));
   };
}}
