
#include "../include/DescriptorTypes.h"
#include "../include/CExif.h"
#include "../include/PropertyTypes.h"

#include "vrdlib/common/include/CContext.h"

#include "vrdlib/utility/include/CStreamReader.h"

#include <exiv2/exiv2.hpp>

#include <log4cxx/logger.h>

namespace VRD
{
namespace Canon
{
   /** \todo Split exif and xmp data into separate classes
    */
   bool CExif::interpret(CContext& context)
   {
      Exiv2::ExifData exif;
      Exiv2::XmpData xmp;
      {
         auto image(Exiv2::ImageFactory::open(context.getReader().getPath().string()));
      
         if (!image->good()) 
         {  throw std::domain_error("Bad image"); }
      
         image->readMetadata();
         exif = image->exifData();
         xmp =  image->xmpData();
         
         if (exif.empty() && xmp.empty())
         {  
            LOG4CXX_WARN(context.getLogger(), "No EXIF and no XMP data found"); 
            return false;
         }
      }
      {
         std::string const key("Xmp.xmp.Rating");
         auto const entry = xmp.findKey(Exiv2::XmpKey(key));
         if (entry != xmp.end())
         {  
            auto const rating(static_cast<int16_t>(entry->toLong()));
            context.setProperty(API::CProperty(to_string(PropertyType::XMP_Rating), API::CProperty::ValueType(rating)));
         }
         else
         {  LOG4CXX_DEBUG(context.getLogger(), key << " not found"); }
      }
      {
         std::string const key("Exif.Canon.VRDOffset");
         auto const entry = exif.findKey(Exiv2::ExifKey(key));
         if (entry != exif.end())
         {
            if (entry->typeId() != Exiv2::TypeId::unsignedLong)
            {  
               std::ostringstream os;
               os << key << " type is: " << entry->typeId() << ", expected is: " << Exiv2::TypeId::unsignedLong << " aka 'unsigned long'";
               throw std::domain_error(os.str());
            }
            
            /* We compare the exif offset value with ours only when 
             * it is available and when it is greater than 0.
             * But when an exif offset value is there, it has 
             * to be equal, otherwise we throw!
             */
            auto const offset(static_cast<std::streampos>(entry->toLong()));
            if (offset > 0)
            {
               auto const vrdPosition(context.getDescriptor(to_string(DescriptorType::VRD)).getPosition());
               if (vrdPosition != Utility::Absolute(offset))
               {  
                  std::ostringstream os;
                  os << key << " does not equal the offset we have determined: Ours: " << vrdPosition << ", EXIF: " << offset;
                  throw std::domain_error(os.str()); 
               }
               
               LOG4CXX_DEBUG(context.getLogger(), key << ": " << offset);
            }
            else
            {  LOG4CXX_DEBUG(context.getLogger(), key << " not set"); }
         }
         else
         {  LOG4CXX_DEBUG(context.getLogger(), key << " not found"); }
      }
      return true;
   }
}}
