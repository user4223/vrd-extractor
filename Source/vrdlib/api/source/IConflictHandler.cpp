
#include "../include/IConflictHandler.h"

namespace VRD
{
namespace API
{

   bool operator==(IConflictHandler::Result const& a, IConflictHandler::Result const& b)
   {  return a.selection == b.selection; }
   
   bool operator!=(IConflictHandler::Result const& a, IConflictHandler::Result const& b)
   {  return a.selection != b.selection; }
   
}}
