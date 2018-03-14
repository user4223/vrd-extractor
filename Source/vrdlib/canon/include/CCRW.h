#pragma once

#include "vrdlib/api/include/IInterpreter.h"

namespace VRD
{
namespace Canon
{
   struct CCRW : API::IInterpreter
   {
      virtual bool interpret(CContext&) override;
   };
}}
