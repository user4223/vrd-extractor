#pragma once

#include "vrdlib/api/include/IPropertySink.h"
#include "vrdlib/api/include/IConflictHandler.h"

#include <boost/filesystem/path.hpp>

#include <log4cxx/logger.h>

#include <memory>

namespace Exiv2 
{ 
   class XmpData; ///< Just to avoid including exiv2 in header
}

namespace VRD
{
   struct CXMPWriter : public API::IPropertySink
   {
      enum class Mode
      {
          Normal
         ,Dry
      };
      
      CXMPWriter(boost::filesystem::path const& path, Mode mode, API::IConflictHandler& conflictHandler);
      virtual ~CXMPWriter();

      bool hasChanged() const;
      void read(boost::filesystem::path const& path);
      void write();

      virtual void setProperty(API::CProperty) override;

   private:
      log4cxx::LoggerPtr m_logger;
      boost::filesystem::path m_path;
      Mode m_mode;
      API::IConflictHandler& m_conflictHandler;
      std::unique_ptr<Exiv2::XmpData> m_xmp;
      bool m_changed;
   };
}
