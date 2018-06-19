#pragma once

#include <vector>
#include <string>
#include <experimental/optional>

#include <boost/filesystem/path.hpp>

namespace VRD
{
namespace API
{
   struct IConflictHandler
   {
      struct Result
      {
         Result(unsigned int s) : selection(s) {}
         Result() : selection() {}
         
         bool doBreak() const 
         {  return !(bool)selection; }
         
         std::experimental::optional<unsigned int> const selection;
      };
      
      virtual ~IConflictHandler() = default;
      
      virtual Result handle(std::vector<std::string> const& options) = 0;
   };
   
   bool operator==(IConflictHandler::Result const& a, IConflictHandler::Result const& b);
   bool operator!=(IConflictHandler::Result const& a, IConflictHandler::Result const& b);
   
   struct IConflictHandlerFactory
   {
      virtual std::unique_ptr<IConflictHandler> create(boost::filesystem::path filePath) = 0;
   };
}}
