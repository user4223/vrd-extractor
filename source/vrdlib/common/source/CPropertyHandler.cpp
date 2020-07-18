
#include "../include/CPropertyHandler.h"

#include <boost/algorithm/string/join.hpp>

#include <ostream>
#include <optional>

namespace VRD
{
   namespace Common
   {
      CPropertyHandler::CPropertyHandler()
          : m_logger(log4cxx::Logger::getLogger("VRD.Common.CPropertyHandler")), m_properties()
      {
      }

      bool CPropertyHandler::setProperty(API::CProperty property)
      {
         auto result(m_properties.emplace(std::make_pair(property.name, property)));
         LOG4CXX_DEBUG(m_logger, result.first->second);
         return result.second;
      }

      CPropertyHandler::OptionalPropertyType CPropertyHandler::getProperty(std::string name) const
      {
         auto const entry(m_properties.find(name));
         if (entry == m_properties.end())
         {
            return OptionalPropertyType();
         }

         return std::make_optional(entry->second);
      }

      std::pair<unsigned int, unsigned int> CPropertyHandler::foreachProperty(std::function<bool(API::CProperty const &)> function) const
      {
         std::pair<unsigned int, unsigned int> count;
         for (auto const &entry : m_properties)
         {
            if (function(entry.second))
            {
               ++count.second;
            }
            ++count.first;
         }
         return count;
      }

      std::string CPropertyHandler::toString() const
      {
         std::vector<std::string> lines;
         foreachProperty([&](auto const &p) { lines.emplace_back(to_string(p)); return true; });
         return boost::join(lines, "\n");
      }
   } // namespace Common
} // namespace VRD
