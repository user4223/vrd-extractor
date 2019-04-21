#pragma once

#include "vrdlib/api/include/IInterpreter.h"

namespace VRD
{
namespace Canon
{
   struct CDR4 : API::IInterpreter
   {
      virtual bool interpret(CContext& context) override;
   };
}}
