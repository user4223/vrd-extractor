#include "../include/CTempDirectoryAwareTestBase.h"

#include <random>

namespace VRD
{
   namespace Test
   {
      static const char charset[] =
          "0123456789"
          "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
      static std::mt19937 generator;

      std::string generateRandomString(std::size_t length)
      {
         std::string result;
         result.resize(length);
         std::sample(std::cbegin(charset),
                     std::cend(charset),
                     std::begin(result),
                     std::intptr_t(length),
                     generator);
         return result;
      }
   }
}
