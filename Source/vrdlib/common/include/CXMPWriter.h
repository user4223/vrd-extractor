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
      CXMPWriter(boost::filesystem::path const& path, std::unique_ptr<API::IConflictHandler> conflictHandler, Mode mode);
      virtual ~CXMPWriter();

      bool hasChanged() const;
      void read(boost::filesystem::path const& path);
      void write();

      virtual bool setProperty(API::CProperty) override;

   private:
      log4cxx::LoggerPtr m_logger;
      boost::filesystem::path m_path;
      std::unique_ptr<API::IConflictHandler> m_conflictHandler;
      Mode m_mode;
      std::unique_ptr<Exiv2::XmpData> m_xmp;
      bool m_changed;
   };
}
