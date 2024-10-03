#pragma once

#include <vector>
#include <string>
#include <optional>

#include <filesystem>

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
            {
               return !(bool)selection;
            }

            std::optional<unsigned int> const selection;
         };

         virtual ~IConflictHandler() = default;

         virtual Result handle(std::vector<std::string> const &options) = 0;
      };

      bool operator==(IConflictHandler::Result const &a, IConflictHandler::Result const &b);
      bool operator!=(IConflictHandler::Result const &a, IConflictHandler::Result const &b);

      struct IConflictHandlerFactory
      {
         virtual ~IConflictHandlerFactory() = default;

         virtual std::unique_ptr<IConflictHandler> create(std::filesystem::path filePath) = 0;
      };
   } // namespace API
} // namespace VRD
