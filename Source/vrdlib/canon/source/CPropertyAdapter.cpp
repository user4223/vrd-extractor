
#include "../include/CPropertyAdapter.h"
#include "../include/DescriptorTypes.h"
#include "../include/PropertyTypes.h"

#include "vrdlib/api/include/PropertyTypes.h"

#include <boost/algorithm/string/join.hpp>

#include <map>
#include <set>
#include <sstream>
#include <stdexcept>

namespace VRD
{
namespace Canon
{ 
   namespace Rating
   {
      template <typename T>
      void addValid(auto& map, auto const& query, PropertyType type, std::map<T,T> validValues)
      {
         if (validValues.empty())
         {  throw std::invalid_argument("List of valid values is empty"); }
         
         auto const property(query.getProperty(to_string(type)));
         if (!property) { return; }
         
         auto const inValue(boost::get<T>(property->value));
         if (validValues.find(inValue) == validValues.end())
         {  return; }
         
         map[validValues.at(inValue)].push_back(*property);
      }
      
      std::vector<std::string> createOptionsList(auto const& map)
      {
         std::vector<std::string> options;
         for (auto const& entry : map)
         {
            std::ostringstream os;
            std::vector<std::string> propertyStrings(entry.second.size());
            std::transform(entry.second.begin(), entry.second.end(), propertyStrings.begin(), [](auto const& p){ return to_string(p); });
            os << API::PropertyType::Rating << ": " << entry.first << " [" << boost::algorithm::join(propertyStrings, ", ") << "]";
            options.emplace_back(os.str());
         }
         return options;
      }
      
      auto const convert([](API::IPropertySource const& query, API::IConflictHandler& conflictHandler)
      {
         std::map<int16_t, std::vector<API::CProperty>> valueMap;
         addValid<uint16_t>(valueMap, query, PropertyType::VRD1_CheckMark, {{1u,5u},{2u,4u},{3u,3}});
         addValid<uint16_t>(valueMap, query, PropertyType::VRD2_CheckMark, {{1u,5u},{2u,4u},{3u,3},{4u,2u},{5u,1u}});
         addValid<uint32_t>(valueMap, query, PropertyType::DR4_CheckMark,  {{1u,5u},{2u,4u},{3u,3},{4u,2u},{5u,1u}});
         addValid<int16_t> (valueMap, query, PropertyType::XMP_Rating,     {{-1,-1},{1,1},{2,2},{3,3},{4,4},{5,5}});
         
         /** All properties are either unset or have the value 0 (unsepcified), so we return 'unset'
          */
         if (valueMap.empty())
         {  return CPropertyAdapter::OptionalPropertyType(); }
         
         /** All valid properties have the same value, so we return exactly that value
          */
         if (valueMap.size() == 1)
         {  return CPropertyAdapter::OptionalPropertyType(API::CProperty(to_string(API::PropertyType::Rating), valueMap.begin()->first)); }
      
         /** We have multiple values, so we ask the conflict handler to solve it
          */
         auto const options(createOptionsList(valueMap));
         auto const selectedValue(conflictHandler.handle(options).selection.value());
         
         /** Selected value is one of the provided options
          */
         auto const selectedEntry(valueMap.find(selectedValue));
         if (selectedEntry != valueMap.end())
         {  return CPropertyAdapter::OptionalPropertyType(API::CProperty(to_string(API::PropertyType::Rating), selectedEntry->first)); }
         
         /** \todo This cannot happen */         
         std::ostringstream os;
         os << "Unresolved conflict, selected value does not exist in option list: " << selectedValue;
         throw std::domain_error(os.str());
      });
   }
   
   auto const propertyMap([]
   {
      std::map<std::string, std::function<CPropertyAdapter::OptionalPropertyType(API::IPropertySource const&, API::IConflictHandler&)>> m;
      m.emplace(std::make_pair(to_string(API::PropertyType::Rating), Rating::convert));
      return m;
   }());   
   
   CPropertyAdapter::CPropertyAdapter(std::unique_ptr<IPropertySource> source, API::IConflictHandler& conflictHandler) 
      :m_source(std::move(source))
      ,m_conflictHandler(conflictHandler)
   {}
   
   CPropertyAdapter::OptionalPropertyType CPropertyAdapter::getProperty(std::string name) const
   {  return propertyMap.at(name)(*m_source, m_conflictHandler); }
   
   unsigned int CPropertyAdapter::foreachProperty(std::function<void(API::CProperty const&)> function) const
   {  
      unsigned int count(0);
      for (auto const& entry : propertyMap)
      {
         auto const optionalProperty(entry.second(*m_source, m_conflictHandler));
         if (optionalProperty)
         {  
            function(optionalProperty.value()); 
            ++count;
         }
      }
      return count;
   }
   
   std::string CPropertyAdapter::toString() const
   {
      std::vector<std::string> lines;
      foreachProperty([&](auto const& p) { lines.emplace_back(to_string(p)); });
      return boost::join(lines, "\n");
   }
}}