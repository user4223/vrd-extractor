
#include "../include/CPropertyHandler.h"

#include <boost/algorithm/string/join.hpp>

#include <ostream>

namespace VRD
{ 
namespace Common
{
   CPropertyHandler::CPropertyHandler() 
      :m_logger(log4cxx::Logger::getLogger("VRD.Common.CPropertyHandler"))
      ,m_properties()
   {}

   void CPropertyHandler::setProperty(API::CProperty property)
   {  
      auto const& entry(m_properties.emplace(std::make_pair(property.name, property)).first->second);
      LOG4CXX_DEBUG(m_logger, entry);
   }

   CPropertyHandler::OptionalPropertyType CPropertyHandler::getProperty(std::string name) const
   {  
      auto const entry(m_properties.find(name));
      if (entry == m_properties.end())
      {  return OptionalPropertyType(); }
      
      return std::experimental::make_optional(entry->second); 
   }

   unsigned int CPropertyHandler::foreachProperty(std::function<void(API::CProperty const&)> function) const
   {  
      unsigned int count(0);
      for (auto const& entry : m_properties)
      {
         function(entry.second); 
         ++count;
      } 
      return count;
   }

   std::string CPropertyHandler::toString() const
   {
      std::vector<std::string> lines;
      foreachProperty([&](auto const& p) { lines.emplace_back(to_string(p)); });
      return boost::join(lines, "\n");
   }
}}
