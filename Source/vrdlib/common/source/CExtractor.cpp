
#include "../include/CExtractor.h"
#include "../include/CXMPWriter.h" ///< \todo remove
#include "../include/CPropertySourceFactory.h"

#include "vrdlib/api/include/PropertyTypes.h"
#include "vrdlib/api/include/IConflictHandler.h" 

#include "vrdlib/utility/include/FileUtility.h"
#include "vrdlib/utility/include/CConflictHandler.h"

#include <iostream>

namespace VRD
{   
   CExtractor::CExtractor(boost::filesystem::path const& root, std::string whiteListRegex, bool dryMode) 
      :m_logger(log4cxx::Logger::getLogger("VRD.CExtractor"))
      ,m_root(root)
   {
      LOG4CXX_INFO(m_logger, "Using path: " << m_root.string());
      
      auto const files(Utility::getNonEmptyMatches(m_root, std::regex(whiteListRegex, std::regex::icase)));
      LOG4CXX_INFO(m_logger, "Number of files matching pattern '" << whiteListRegex << "': " << files.size());
      
      size_t updatedFiles = 0;
      Utility::CManualConflictHandler conflictHandler(std::cin, std::cout);
      Common::CPropertySourceFactory propertySourceFactory(conflictHandler);
      for (auto const& inFile : files) 
      { 
         auto outFile(inFile);
         outFile.replace_extension(outFile.extension().string() + ".xmp");
         
         LOG4CXX_DEBUG(m_logger, "Input file: " << inFile.string());
         LOG4CXX_DEBUG(m_logger, "Output file: " << outFile.string());
         
         /** \todo Add factory for sinks and remove concrete instances here.
          */
         CXMPWriter writer(outFile, dryMode ? CXMPWriter::Mode::Dry : CXMPWriter::Mode::Normal, conflictHandler);
         auto const changedProperties(propertySourceFactory.create(inFile)->foreachProperty([&](API::CProperty const& property)
         {  writer.setProperty(property); }));
         
         if (writer.hasChanged()) 
         {
            ++updatedFiles;
            LOG4CXX_INFO(m_logger, changedProperties << " XMP properties changed, updating file: " << outFile.string());
         }
         else
         {  LOG4CXX_DEBUG(m_logger, "No XMP properties changed, skipping file: " << outFile.string()); }
      }
      
      LOG4CXX_INFO(m_logger, "Number of processed files: " << files.size());
      LOG4CXX_INFO(m_logger, "Number of updated files: " << updatedFiles);
      if (dryMode) 
      {  LOG4CXX_WARN(m_logger, "Dry mode was enabled, so we changed nothing effectively!"); }
   }
}
