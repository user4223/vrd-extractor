#pragma once

#include "vrdlib/api/include/IConflictHandler.h"

#include <vector>
#include <string>
#include <iosfwd>
#include <functional>

namespace VRD
{
namespace Utility
{
   /** \todo Add a pre-defined conflict resolution strategy 'TakeAlwaysHighest'
    */
    
   struct AConflictHandlerBase : public API::IConflictHandler
   {
   protected:
      int validate(int, int);
   };
   
   struct CManualConflictHandler : public AConflictHandlerBase
   {      
      CManualConflictHandler(std::istream& is, std::ostream& os, boost::filesystem::path filePath);
      
      virtual Result handle(std::vector<std::string> const& options);

   private:
      std::istream& m_is;
      std::ostream& m_os;
      boost::filesystem::path m_filePath;
   };
   
   struct CManualConflictHandlerFactory : public API::IConflictHandlerFactory
   {
      CManualConflictHandlerFactory(std::istream& is, std::ostream& os);
      
      virtual std::unique_ptr<API::IConflictHandler> create(boost::filesystem::path filePath) override;
   
   private:
      std::istream& m_is;
      std::ostream& m_os;
   };
   
   struct CCustomConflictHandler : public AConflictHandlerBase
   {
      typedef std::function<int(std::vector<std::string> const&)> FunctionType;
      
      CCustomConflictHandler(FunctionType f);
      
      virtual Result handle(std::vector<std::string> const& options);
      
   private:
      FunctionType m_f;
   };
}}
