
#include "../include/FileUtility.h"

#include <regex>

namespace VRD
{
   namespace Utility
   {
      std::set<std::filesystem::path> getNonEmptyMatches(std::filesystem::path root, std::regex filePattern)
      {
         std::set<std::filesystem::path> paths;
         if (!std::filesystem::is_directory(root))
         {
            if (std::regex_match(root.string(), filePattern))
            {
               paths.insert(root);
            }
            return paths;
         }

         /** \todo Add option to ignore hidden files,
                   because sometimes there are hidden temporary files
                   with the same extension leading to errors when they are interpreted as
                   normal source files!
          */

         auto const currentPath(std::filesystem::current_path());
         for (auto &&entry : std::filesystem::recursive_directory_iterator(root))
         {
            auto const &path(entry.path());

            if (std::filesystem::is_directory(entry))
            {
               continue;
            }
            else if (std::filesystem::is_empty(path))
            {
               continue;
            }

            if (std::regex_match(path.string(), filePattern))
            {
               paths.insert(std::filesystem::relative(path, currentPath));
            }
         }
         return paths;
      }
   }
}
