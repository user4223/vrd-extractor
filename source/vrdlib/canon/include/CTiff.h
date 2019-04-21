#pragma once

#include "vrdlib/api/include/IInterpreter.h"

namespace VRD
{
   /** This checks just some fundamental and simple TIFF stuff
    *  and this is here to avoid the dependency to libtiff. 
    *  When it comes out it is required 
    *  more than checking the first and the last bytes 
    *  in future, maybe we should take the 
    *  dependency and use libtiff to avoid duplication
    *  complexity we just cannot estimate. However,
    *  as long as it keeps simple as it is currently,
    *  I would avoid the dependency.
    */
   struct CTiff : API::IInterpreter
   {
      virtual bool interpret(CContext&) override;
   };
}