#pragma once

#include "vrdlib/utility/include/PositionUtility.h"

#include <iosfwd>
#include <string>

namespace VRD
{
   struct CDescriptor
   {
      static std::string::value_type const separator;
            
      CDescriptor(std::string name, Utility::Absolute position, size_t headerSize, size_t payloadSize, std::string description = "");
      CDescriptor(CDescriptor&&) = default;
      CDescriptor(CDescriptor const&) = default;
      CDescriptor& operator=(CDescriptor&&) = default;
      CDescriptor& operator=(CDescriptor const&) = default;
      
      std::string const& getName() const { return m_name; }
      std::string const& getDescription() const { return m_description; }
      
      Utility::Absolute getPosition() const { return m_position; }
      Utility::Absolute getPayloadPosition() const { return m_position + m_headerSize; }
      Utility::Absolute getEnd() const { return getPosition() + getSize(); }
      
      size_t getSize() const { return m_headerSize + m_payloadSize; }
      size_t getPayloadSize() const { return m_payloadSize; }
      
      std::string toString() const;
      
   private:
      std::string       const m_name;
      Utility::Absolute const m_position;
      size_t            const m_headerSize;
      size_t            const m_payloadSize;
      std::string       const m_description;
   };

   std::ostream& operator<<(std::ostream& os, CDescriptor const&);
}