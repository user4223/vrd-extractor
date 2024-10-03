#pragma once

#include <filesystem>

#include <set>
#include <regex>

namespace VRD
{
    namespace Utility
    {
        /** Returns all non-empty and regex matching files below the
            given path. When the given path is a path to a file, the
            result list will contain it when it is matching the pattern.

            \param path to directory of file to be searched through recursively
            \param filePattern to be used to select files for result set
        */
        std::set<std::filesystem::path> getNonEmptyMatches(std::filesystem::path path, std::regex filePattern);
    }
}
