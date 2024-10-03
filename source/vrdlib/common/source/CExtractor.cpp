
#include "../include/CExtractor.h"
#include "../include/CPropertySourceFactory.h"
#include "../include/CPropertySinkFactory.h"

#include "vrdlib/api/include/PropertyTypes.h"

#include "vrdlib/utility/include/FileUtility.h"

#include <iostream>

namespace std
{
   std::ostream &operator<<(std::ostream &os, std::pair<unsigned int, unsigned int> const &p)
   {
      if (p.first == 0 && p.second == 0)
      {
         os << "0  ";
      }
      else
      {
         os << p.first << "/" << p.second;
      }
      return os;
   }

   std::pair<unsigned int, unsigned int> operator+(std::pair<unsigned int, unsigned int> a, std::pair<unsigned int, unsigned int> const &b)
   {
      a.first += b.first;
      a.second += b.second;
      return a;
   }
}

namespace VRD
{
   CExtractor::CExtractor(std::filesystem::path const &root, std::string whiteListRegex, std::unique_ptr<VRD::API::IConflictHandlerFactory> conflictHandlerFactory, bool dryMode)
       : m_logger(log4cxx::Logger::getLogger("VRD.CExtractor")), m_root(root)
   {
      LOG4CXX_INFO(m_logger, "Using root path: " << m_root.string());

      auto const files(Utility::getNonEmptyMatches(m_root, std::regex(whiteListRegex, std::regex::icase)));
      LOG4CXX_INFO(m_logger, "Number of files matching pattern '" << whiteListRegex << "': " << files.size());

      auto processedFiles(std::make_pair(0u, 0u));
      Common::CPropertySourceFactory propertySourceFactory(*conflictHandlerFactory);
      Common::CPropertySinkFactory propertySinkFactory(*conflictHandlerFactory, dryMode ? API::IPropertySink::Mode::Dry : API::IPropertySink::Mode::Normal);
      for (auto const &inFile : files)
      {
         auto outFile(inFile);
         outFile.replace_extension(outFile.extension().string() + ".xmp");

         LOG4CXX_DEBUG(m_logger, "Input file:  " << inFile.string());
         LOG4CXX_DEBUG(m_logger, "Output file: " << outFile.string());

         auto const propertySource(propertySourceFactory.create(inFile));
         auto propertySink(propertySinkFactory.create(outFile));

         auto const processedProperties(propertySource->foreachProperty([&](auto const &property)
                                                                        { return propertySink->setProperty(property); }));

         processedFiles = processedFiles + std::make_pair(1u, processedProperties.second > 0 ? 1u : 0u);

         std::ostringstream os;
         os << processedProperties.first << " XMP properties available, ";
         if (processedProperties.first > 0)
         {
            os << "changed " << processedProperties.second << ", ";
         }
         if (processedProperties.second > 0)
         {
            os << "UPDATING";
         }
         else
         {
            os << "skipping";
         }
         os << " file: " << outFile.string();

         LOG4CXX_INFO(m_logger, os.str());
      }

      LOG4CXX_INFO(m_logger, "Files processed/updated: " << processedFiles);
      if (dryMode)
      {
         LOG4CXX_WARN(m_logger, "Dry mode was enabled, so we changed nothing effectively!");
      }
   }
}
