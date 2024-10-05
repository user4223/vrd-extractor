
#include "../include/CPropertySinkFactory.h"
#include "../include/CXMPWriter.h"

#include "vrdlib/api/include/IConflictHandler.h"

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
      std::map<std::string, std::function<std::unique_ptr<API::IPropertySink>(std::filesystem::path, std::unique_ptr<API::IConflictHandler>, API::IPropertySink::Mode)>> m;
      m.emplace(std::make_pair(".xmp", [](auto path, auto conflictHandler, auto mode) { return std::make_unique<CXMPWriter>(path, std::move(conflictHandler), mode); }));
      
      /** Add further supported file extensions here.
       */
      return m; }());

      CPropertySinkFactory::CPropertySinkFactory(API::IConflictHandlerFactory &conflictHandlerFactory, API::IPropertySink::Mode mode)
          : m_conflictHandlerFactory(conflictHandlerFactory), m_mode(mode)
      {
      }

      std::unique_ptr<API::IPropertySink> CPropertySinkFactory::create(std::filesystem::path filePath)
      {
         auto const extension(boost::algorithm::to_lower_copy(filePath.extension().string()));
         auto const entry(map.find(extension));
         if (entry != map.end())
         {
            return entry->second(filePath, m_conflictHandlerFactory.create(filePath), m_mode);
         }

         std::vector<std::string> extensions;
         std::transform(map.begin(), map.end(), std::back_inserter(extensions), [](auto const &e)
                        { return e.first; });
         std::ostringstream os;
         os << "Unsupported file extension '" << extension << "' requested, supported are: '" << boost::algorithm::join(extensions, "', '") << "'";
         throw std::invalid_argument(os.str());
      }
   }
}
