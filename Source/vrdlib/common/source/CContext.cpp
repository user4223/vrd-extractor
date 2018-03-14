
#include "../include/CContext.h"

#include "vrdlib/utility/include/CStreamReader.h"

#include <boost/algorithm/string/join.hpp>

#include <ostream>

namespace VRD
{  
   CContext::CContext(Utility::CStreamReader& reader) 
      :Common::CPropertyHandler()
      ,m_pReader()
      ,m_reader(reader)
      ,m_logger(log4cxx::Logger::getLogger("VRD.CContext"))
      ,m_descriptors()
   {}
   
   CContext::CContext(std::unique_ptr<Utility::CStreamReader> reader)
      :Common::CPropertyHandler()
      ,m_pReader(std::move(reader))
      ,m_reader(*m_pReader)
      ,m_logger(log4cxx::Logger::getLogger("VRD.CContext"))
      ,m_descriptors()
   {}
      
   CDescriptor const& CContext::setDescriptor(CDescriptor descriptor)
   {  
      auto const& entry(m_descriptors.emplace(std::make_pair(descriptor.getName(), std::move(descriptor))).first->second); 
      LOG4CXX_DEBUG(m_logger, entry);
      return entry;
   }
   
   CDescriptor CContext::getDescriptor(std::string name) const
   {  return m_descriptors.at(name); }
   
   void CContext::foreachDescriptor(std::function<void(CDescriptor const&)> f) const
   {  std::for_each(m_descriptors.begin(), m_descriptors.end(), [=](auto const& entry){ f(entry.second); }); }
   
   std::ostream& operator<<(std::ostream& os, CContext const& c)
   {
      std::vector<std::string> lines;
      c.foreachDescriptor([&](auto const& d) { lines.emplace_back(d.toString()); });
      
      os << boost::join(lines, "\n") 
         << c.CPropertyHandler::toString();
      return os;
   }
}
