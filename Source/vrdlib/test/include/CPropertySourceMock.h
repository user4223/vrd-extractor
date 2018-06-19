#pragma once

#include "vrdlib/api/include/IPropertySource.h"

#include <gmock/gmock.h>

namespace VRD
{
namespace Test
{
   struct CPropertySourceMock : API::IPropertySource
   {
      MOCK_CONST_METHOD1(getProperty, OptionalPropertyType(std::string name));
      MOCK_CONST_METHOD1(foreachProperty, std::pair<unsigned int, unsigned int>(std::function<bool(API::CProperty const&)> f));
      MOCK_CONST_METHOD0(toString, std::string());
   };
}}
