#pragma once

#include "vrdlib/api/include/IInterpreter.h"

namespace VRD
{
namespace Canon
{
   struct CCR2 : API::IInterpreter
   {
      virtual bool interpret(CContext&) override;
   };
}}
