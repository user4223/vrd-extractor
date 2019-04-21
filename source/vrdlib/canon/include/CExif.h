#pragma once

#include "vrdlib/api/include/IInterpreter.h"

namespace VRD
{
namespace Canon
{
   struct CExif : API::IInterpreter
   {
      virtual bool interpret(CContext&) override;
   };
}}
