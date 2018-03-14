
#include "../include/FileUtility.h"

#include <boost/filesystem.hpp>

#include <regex>

namespace VRD
{
namespace Utility
{
   std::set<boost::filesystem::path> getNonEmptyMatches(boost::filesystem::path root, std::regex filePattern)
   {
      std::set<boost::filesystem::path> paths;
      if (!boost::filesystem::is_directory(root))
      {
         if (std::regex_match(root.string(), filePattern)) { paths.insert(root); }
         return paths;
      }
      
      auto const currentPath(boost::filesystem::current_path());
      for(auto&& entry : boost::filesystem::recursive_directory_iterator(root)) 
      {
         auto const& path(entry.path());
         
         if       (boost::filesystem::is_directory(entry)) { continue; }
         else if  (boost::filesystem::is_empty(path))      { continue; }
         
         if (std::regex_match(path.string(), filePattern)) 
         {  paths.insert(boost::filesystem::relative(path, currentPath)); }
      }
      return paths;
   }
}}
