#pragma once

#include "vrdlib/common/include/CPropertyHandler.h"

#include "CDescriptor.h"

#include <log4cxx/logger.h>

#include <map>
#include <iosfwd>

namespace VRD
{       
   namespace Utility { struct CStreamReader; }
   
   struct CContext : public Common::CPropertyHandler
   {      
      CContext(Utility::CStreamReader& reader);
      CContext(std::unique_ptr<Utility::CStreamReader> reader);
      
      CContext(CContext const&) = delete;
      CContext(CContext&&) = delete;
      
      CContext& operator=(CContext const&) = delete;
      CContext& operator=(CContext&&) = delete;
      
      log4cxx::LoggerPtr getLogger() const { return m_logger; }
      Utility::CStreamReader& getReader() const { return m_reader; }
      
      CDescriptor const& setDescriptor(CDescriptor descriptor);
      CDescriptor getDescriptor(std::string name) const;
      void foreachDescriptor(std::function<void(CDescriptor const&)> f) const;
           
   private:
      std::unique_ptr<Utility::CStreamReader> m_pReader;
      Utility::CStreamReader& m_reader;
      mutable log4cxx::LoggerPtr m_logger;
      std::map<std::string, CDescriptor> m_descriptors;
   };
   
   std::ostream& operator<<(std::ostream& os, CContext const&);
}
