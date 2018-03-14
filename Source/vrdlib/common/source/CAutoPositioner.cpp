
#include "../include/CAutoPositioner.h"

#include "vrdlib/utility/include/CStreamReader.h"

namespace VRD
{
   CAutoPositioner::CAutoPositioner(Utility::CStreamReader& reader, CDescriptor const& descriptor)
      :m_reader(reader)
      ,m_descriptor(descriptor)
      ,m_variableSize(0) 
   {  m_reader.moveTo(m_descriptor.getPayloadPosition()); }
   
   CAutoPositioner::~CAutoPositioner()
   {  m_reader.moveTo(m_descriptor.getPayloadPosition() + m_descriptor.getPayloadSize() + m_variableSize); }
      
   void CAutoPositioner::addSize(uint32_t s) 
   {  m_variableSize += s; }
}