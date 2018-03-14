#pragma once

#include "vrdlib/api/include/IInterpreter.h"

namespace VRD
{
namespace Canon
{
   struct CVRD : API::IInterpreter
   {
      virtual bool interpret(CContext&) override;
   };
}}
