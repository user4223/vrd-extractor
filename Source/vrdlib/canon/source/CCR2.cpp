
#include "../include/CCR2.h"
#include "../include/CExif.h"
#include "../include/CTiff.h"
#include "../include/CVRD.h"

#include "vrdlib/common/include/CContext.h"

#include <stdexcept>

namespace VRD
{
namespace Canon
{
   bool CCR2::interpret(CContext& context)
   {      
      if (!CTiff().interpret(context))
      {  throw std::domain_error("Not a valid TIFF file, skipping"); }

      CVRD().interpret(context);
      CExif().interpret(context);
      return true;
   }
}}
