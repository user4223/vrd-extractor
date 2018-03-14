
#include "../include/CEdit4.h"
#include "../include/CDR4.h"
#include "../include/DescriptorTypes.h"
#include "../include/CDirectoryEntryHeader.h"

#include "vrdlib/common/include/CContext.h"
#include "vrdlib/common/include/CAutoPositioner.h"

#include <vector>

namespace VRD
{
namespace Canon
{   
   auto const edit4Entries([]
   {
      std::vector<std::unique_ptr<API::IInterpreter>> v;
      v.emplace_back(std::make_unique<CDR4>());
      return v;
   }());
   
   bool CEdit4::interpret(CContext& context)
   {     
      auto& reader(context.getReader());
      CDirectoryEntryHeader const header(reader);
      
      auto const& descriptor(context.setDescriptor(CDescriptor(
          to_string(DescriptorType::Edit4)
         ,header.position, header.size, header.payloadSize, header.description)));
      
      CAutoPositioner reposition(reader, descriptor);
      for (auto& entry : edit4Entries) { entry->interpret(context); }
      return true;
   }  
}}
