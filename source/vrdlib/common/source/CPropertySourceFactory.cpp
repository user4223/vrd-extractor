
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
      std::map<std::string, std::function<void(CContext&)>> m;
      m.emplace(std::make_pair(".cr2", [](auto& context) { Canon::CCR2().interpret(context); }));
      m.emplace(std::make_pair(".vrd", [](auto& context) { Canon::CVRD().interpret(context); }));
      m.emplace(std::make_pair(".dr4", [](auto& context) { Canon::CDR4().interpret(context); }));
      m.emplace(std::make_pair(".crw", [](auto& context) { Canon::CCRW().interpret(context); }));
      
      /** Add further supported file extensions and interpreters here.
       */
      return m;
   }());
   
   CPropertySourceFactory::CPropertySourceFactory(API::IConflictHandlerFactory& conflictHandlerFactory)
      :m_conflictHandlerFactory(conflictHandlerFactory)
   {}
   
   std::unique_ptr<API::IPropertySource> CPropertySourceFactory::create(boost::filesystem::path filePath)
   {
      auto const extension(boost::algorithm::to_lower_copy(filePath.extension().string()));
      auto const entry(map.find(extension));
      if (entry != map.end())
      {  
         auto context(std::make_unique<CContext>(std::make_unique<Utility::CStreamReader>(filePath.string())));
         entry->second(*context);
         return std::make_unique<Canon::CPropertyAdapter>(std::move(context), m_conflictHandlerFactory.create(filePath));
      }
      
      std::vector<std::string> extensions;
      std::transform(map.begin(), map.end(), std::back_inserter(extensions), [](auto const& e){ return e.first; });
      std::ostringstream os;
      os << "Unsupported file extension '" << extension << "' requested, supported are: '" << boost::algorithm::join(extensions, "', '") << "'";
      throw std::invalid_argument(os.str());
   }
}}
