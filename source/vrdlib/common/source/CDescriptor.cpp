
#include "../include/CDescriptor.h"

#include <ios>
#include <sstream>

namespace VRD
{
   std::string::value_type const CDescriptor::separator = '.';

   CDescriptor::CDescriptor(std::string name, Utility::Absolute position, size_t headerSize, size_t payloadSize, std::string description)
       : m_name(name), m_position(position), m_headerSize(headerSize), m_payloadSize(payloadSize), m_description(description)
   {
   }

   std::string CDescriptor::toString() const
   {
      std::ostringstream os;
      auto const &description(getDescription());
      os << getName()
         << ": Position: " << getPosition()
         << ", Payload: " << getPayloadPosition()
         << ", Size: " << getPayloadSize()
         << (description.empty() ? "" : ", Description: " + description);
      return os.str();
   }

   std::ostream &operator<<(std::ostream &os, CDescriptor const &d)
   {
      return os << d.toString();
   }
}
