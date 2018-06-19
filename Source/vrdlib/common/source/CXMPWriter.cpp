
#include "../include/CXMPWriter.h"

#include <boost/filesystem/operations.hpp>

#include <exiv2/exiv2.hpp>

namespace VRD
{
   CXMPWriter::CXMPWriter(boost::filesystem::path const& path, std::unique_ptr<API::IConflictHandler> conflictHandler, Mode mode)
      :m_logger(log4cxx::Logger::getLogger("VRD.CXMPWriter"))
      ,m_path(path)
      ,m_conflictHandler(std::move(conflictHandler))
      ,m_mode(mode)
      ,m_xmp(std::make_unique<Exiv2::XmpData>())
      ,m_changed(false)
   {  
      if (!boost::filesystem::exists(m_path))
      {  return; }
      
      read(m_path); 
   }

   CXMPWriter::~CXMPWriter()
   {
      if (!hasChanged())
      {  return; }
      
      try                              { write(); }
      catch (std::exception const& e)  { LOG4CXX_ERROR(m_logger, "Write failed with: " << e.what()); }
   }
      
   void CXMPWriter::read(boost::filesystem::path const& path)
   {
      std::ifstream stream(m_path.string().c_str());
      stream.seekg(0, std::ios::end);
      auto const size(stream.tellg());
      std::string packet(size, ' ');
      stream.seekg(0, std::ios::beg);
      stream.read(&packet[0], size); 
      
      if (Exiv2::XmpParser::decode(*m_xmp, packet) != 0)
      {  throw std::domain_error("Decoding xmp failed: " + m_path.string()); }
      
      LOG4CXX_DEBUG(m_logger, "Found and decoded existing XMP file: " << m_path.string());
   }
   
   void CXMPWriter::write()
   {
      if (m_xmp->empty())
      {  
         LOG4CXX_DEBUG(m_logger, "Empty XMP data, skip writing file: " << m_path.string());
         return; 
      }
      
      if (!hasChanged())
      {  
         LOG4CXX_DEBUG(m_logger, "Unchanged XMP data, skip writing file: " << m_path.string());
         return; 
      }
      
      std::string packet;
      if(Exiv2::XmpParser::encode(packet, *m_xmp, Exiv2::XmpParser::useCompactFormat | Exiv2::XmpParser::omitPacketWrapper) != 0)
      {  throw std::domain_error("Encoding xmp failed"); }
      
      if (m_mode == Mode::Dry)
      {  LOG4CXX_DEBUG(m_logger, "Dry mode enabled, skipping write"); }
      else
      {  
         std::ofstream stream(m_path.string().c_str());
         stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" << packet; 
      }
 
      LOG4CXX_DEBUG(m_logger, "Encoded and written XMP file: " << m_path.string());
      m_changed = false;
   }
   
   bool CXMPWriter::setProperty(API::CProperty property)
   {
      Exiv2::XmpKey const key(property.name);
      Exiv2::XmpTextValue const value(API::to_string(property.value));
      Exiv2::XmpData::iterator const existingEntry(m_xmp->findKey(key));
      
      auto const update([this, &key, &value, &property]()
      {
         if (m_xmp->add(key, &value) != 0)
         {  throw std::domain_error("Failed to add XMP entry: " + key.key()); }
         
         LOG4CXX_DEBUG(m_logger, "Added property: " << property);
         return (m_changed = true);
      });      
      
      if(existingEntry != m_xmp->end()) 
      {
         auto const existingValueString(existingEntry->toString());
         if (existingValueString.compare(value.toString()) != 0)
         {
            LOG4CXX_DEBUG(m_logger, "Try solving a conflict for key: " << key.key());
            
            std::vector<std::string> options;
            options.push_back(property.name + ": " + existingValueString + " [Existing in destination]");
            options.push_back(property.name + ": " + value.toString() +    " [Imported from source]");
            auto const selectedValue(m_conflictHandler->handle(options).selection.value());
            
            if (selectedValue == 0) ///< Keep existing value
            {  return false; }
            else
            if (selectedValue == 1) ///< Overwrite with imported value
            {  return update(); }
            
            /** \todo This cannot happen */
            std::ostringstream os;
            os << "Unresolved conflict, selected value does not exist in option list: " << selectedValue;
            throw std::domain_error(os.str());
         }
         
         LOG4CXX_DEBUG(m_logger, "Found existing key with same value, skipping key: " << key.key() << ", Value: " << value);
         return false;
      }

      return update();
   }
   
   bool CXMPWriter::hasChanged() const { return m_changed; }
}
