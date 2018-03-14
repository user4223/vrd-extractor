#pragma once

namespace VRD
{
   struct CContext;

namespace API
{
   struct IInterpreter
   {
      virtual ~IInterpreter() = default;
      
      /** Interprete implementation defined content,
       *  retrieve required information from context and store 
       *  information retrieved from domain to the context.
       * 
       *  \param context to retrieve required information and to store information from domain
       * 
       *  \todo Think whether we do need the return value! There is only one place it makes sense, 
       *         maybe we can solve it there differently and then remove it from this interface!
       */
      virtual bool interpret(CContext& context) = 0;
   };
}}
