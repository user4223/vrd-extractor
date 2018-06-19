
#include "../include/CExtractor.h"
#include "../include/CPropertySourceFactory.h"
#include "../include/CPropertySinkFactory.h"

#include "vrdlib/api/include/PropertyTypes.h" 

#include "vrdlib/utility/include/FileUtility.h"

#include <iostream>

namespace VRD
{   
   CExtractor::CExtractor(boost::filesystem::path const& root, std::string whiteListRegex, std::unique_ptr<VRD::API::IConflictHandlerFactory> conflictHandlerFactory, bool dryMode) 
      :m_logger(log4cxx::Logger::getLogger("VRD.CExtractor"))
      ,m_root(root)
   {
      LOG4CXX_INFO(m_logger, "Using root path: " << m_root.string());
      
      auto const files(Utility::getNonEmptyMatches(m_root, std::regex(whiteListRegex, std::regex::icase)));
      LOG4CXX_INFO(m_logger, "Number of files matching pattern '" << whiteListRegex << "': " << files.size());
      
      size_t updatedFiles = 0;
      Common::CPropertySourceFactory propertySourceFactory(*conflictHandlerFactory);
      Common::CPropertySinkFactory propertySinkFactory(*conflictHandlerFactory, dryMode ? API::IPropertySink::Mode::Dry : API::IPropertySink::Mode::Normal);
      for (auto const& inFile : files) 
      { 
         auto outFile(inFile);
         outFile.replace_extension(outFile.extension().string() + ".xmp");
         
         LOG4CXX_DEBUG(m_logger, "Input file:  " << inFile.string());
         LOG4CXX_DEBUG(m_logger, "Output file: " << outFile.string());
         
         auto propertySource(propertySourceFactory.create(inFile));
         auto propertySink(propertySinkFactory.create(outFile));
         
         auto const result(propertySource->foreachProperty([&](auto const& property)
         {  return propertySink->setProperty(property); }));
         
         if (result.second) 
         {
            ++updatedFiles;
            LOG4CXX_INFO(m_logger, result.first << "/" << result.second
               << " XMP properties available/changed, updating file: " << outFile.string());
         }
         else
         {  LOG4CXX_INFO(m_logger, "No XMP properties changed, skipping file: " << outFile.string()); }
      }
      
      LOG4CXX_INFO(m_logger, "Number of processed files: " << files.size());
      LOG4CXX_INFO(m_logger, "Number of updated files:   " << updatedFiles);
      if (dryMode) 
      {  LOG4CXX_WARN(m_logger, "Dry mode was enabled, so we changed nothing effectively!"); }
   }
}
