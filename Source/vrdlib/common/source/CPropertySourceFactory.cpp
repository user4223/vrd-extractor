
#include "../include/CPropertySourceFactory.h"

#include "vrdlib/api/include/IConflictHandler.h"

#include "vrdlib/canon/include/CCR2.h"
#include "vrdlib/canon/include/CCRW.h"
#include "vrdlib/canon/include/CVRD.h"
#include "vrdlib/canon/include/CDR4.h"
#include "vrdlib/canon/include/CPropertyAdapter.h"

#include "vrdlib/common/include/CContext.h"

#include "vrdlib/utility/include/CStreamReader.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/join.hpp>

#include <map>
#include <stdexcept>

namespace VRD
{
namespace Common
{  
   auto const map([]
   {
      std::map<std::string, std::function<std::unique_ptr<API::IPropertySource>(boost::filesystem::path, API::IConflictHandler&)>> m;
      m.emplace(std::make_pair(".cr2", [](auto path, auto& conflictHandler)
      {
         auto context(std::make_unique<CContext>(std::make_unique<Utility::CStreamReader>(path.string())));
         Canon::CCR2().interpret(*context);
         return std::make_unique<Canon::CPropertyAdapter>(std::move(context), conflictHandler);
      }));
      m.emplace(std::make_pair(".vrd", [](auto path, auto& conflictHandler)
      {
         auto context(std::make_unique<CContext>(std::make_unique<Utility::CStreamReader>(path.string())));
         Canon::CVRD().interpret(*context);
         return std::make_unique<Canon::CPropertyAdapter>(std::move(context), conflictHandler);
      }));
      m.emplace(std::make_pair(".dr4", [](auto path, auto& conflictHandler)
      {
         auto context(std::make_unique<CContext>(std::make_unique<Utility::CStreamReader>(path.string())));
         Canon::CDR4().interpret(*context);
         return std::make_unique<Canon::CPropertyAdapter>(std::move(context), conflictHandler);
      }));
      m.emplace(std::make_pair(".crw", [](auto path, auto& conflictHandler)
      {
         auto context(std::make_unique<CContext>(std::make_unique<Utility::CStreamReader>(path.string())));
         Canon::CCRW().interpret(*context);
         return std::make_unique<Canon::CPropertyAdapter>(std::move(context), conflictHandler);
      }));
      
      /** Add further supported file extensions and interpreters here.
       */
      return m;
   }());
   
   CPropertySourceFactory::CPropertySourceFactory(API::IConflictHandler& conflictHandler)
      :m_conflictHandler(conflictHandler)
   {}
   
   std::unique_ptr<API::IPropertySource> CPropertySourceFactory::create(boost::filesystem::path filePath)
   {
      auto const extension(boost::algorithm::to_lower_copy(filePath.extension().string()));
      auto const entry(map.find(extension));
      if (entry != map.end())
      {  return entry->second(filePath, m_conflictHandler); }
      
      std::vector<std::string> extensions;
      std::transform(map.begin(), map.end(), std::back_inserter(extensions), [](auto const& e){ return e.first; });
      std::ostringstream os;
      os << "Unsupported file extension '" << extension << "' requested, supported are: '" << boost::algorithm::join(extensions, "', '") << "'";
      throw std::invalid_argument(os.str());
   }
}}
