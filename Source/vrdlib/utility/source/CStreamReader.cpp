
#include "../include/CStreamReader.h"

#include <boost/filesystem/operations.hpp>

#include <fstream>
#include <stdexcept>

namespace VRD
{
namespace Utility
{
   namespace detail
   {
      template <typename T>
      T read(std::istream& is)
      {
         T v(0);
         is.read(reinterpret_cast<char*>(&v), sizeof(v));
         if (!is.good())
         {  throw std::domain_error("Reading stream failed"); }
         return v;
      }

      template <typename T> T swapBytes(T v)
      {
         auto r(reinterpret_cast<char*>(&v));
         std::reverse(r, r + sizeof(T));
         return v;
      }
      
      template <typename T>
      T readAndSwapBytes(std::istream& is) { return swapBytes(read<T>(is)); }
   }

   std::ostream& operator<<(std::ostream& os, ByteOrder order)
   {
      switch (order)
      {
         case ByteOrder::LittleEndian: return os << "little endian";
         case ByteOrder::BigEndian:    return os << "big endian";
      }
      throw std::domain_error("Invalid byte order");
   }
   
   CStreamReader::CStreamReader(boost::filesystem::path const& path, Utility::ByteOrder byteOrder)
      :m_path(path)
      ,m_raw([&]
      {
         if (boost::filesystem::is_directory(path))
         {  throw std::invalid_argument(std::string("Path to read from is a directory path: ") + path.string()); }
         return std::make_unique<std::ifstream>(path.string(), std::ios::binary);
      }())
      ,m_streamOrder(byteOrder)
      ,m_hostOrder([] ///< \todo Do we need host byte order here?
      {
         int i=1;
         return (int)*((unsigned char *)&i)==1 ? Utility::ByteOrder::LittleEndian: Utility::ByteOrder::BigEndian;
      }())
      ,m_size(0)
   {
      moveToEnd();
      m_size = m_raw->tellg();
      moveToBegin();
   }
   
   template <typename T> T CStreamReader::read()
   {
      //if (m_streamOrder != m_hostOrder)
      if (m_streamOrder == Utility::ByteOrder::LittleEndian)
      {  return detail::readAndSwapBytes<T>(*m_raw); }
      
      return detail::read<T>(*m_raw);
   }
   template<> std::string CStreamReader::read<std::string>()
   {
      std::string result;
      while (1) ///< Until \0 has been found, should we do?
      {
         auto const v(detail::read<std::string::value_type>(*m_raw));
         if (v == 0) { break; }
         result.push_back(v);
      }
      return result;
   }
   template double      CStreamReader::read();
   template <> uint8_t  CStreamReader::read()
   {  return detail::read<uint8_t>(*m_raw); }
   template uint16_t    CStreamReader::read();
   template uint32_t    CStreamReader::read();
   template <> int8_t   CStreamReader::read()
   {  return detail::read<int8_t>(*m_raw); }
   template int16_t     CStreamReader::read();
   template int32_t     CStreamReader::read();
   
   template <typename T> T CStreamReader::readRaw()
   {  return detail::read<T>(*m_raw); }
   template<> std::string CStreamReader::readRaw<std::string>()
   {  return read<std::string>(); }
   template double      CStreamReader::readRaw();
   template uint8_t     CStreamReader::readRaw();
   template uint16_t    CStreamReader::readRaw();
   template uint32_t    CStreamReader::readRaw();
   template int8_t      CStreamReader::readRaw();
   template int16_t     CStreamReader::readRaw();
   template int32_t     CStreamReader::readRaw();
   
   template <typename T> T CStreamReader::readAt(Absolute position)
   {      
      auto current(getPosition());
      moveTo(position);
      auto value(read<T>());
      moveTo(current);
      return value;
   }
   template std::string CStreamReader::readAt(Absolute);
   template double      CStreamReader::readAt(Absolute);
   template uint8_t     CStreamReader::readAt(Absolute);
   template uint16_t    CStreamReader::readAt(Absolute);
   template uint32_t    CStreamReader::readAt(Absolute);
   template int8_t      CStreamReader::readAt(Absolute);
   template int16_t     CStreamReader::readAt(Absolute);
   template int32_t     CStreamReader::readAt(Absolute);
   
   template <typename T> T CStreamReader::readAt(Relative offset)
   {  return readAt<T>(getPosition() + offset); }   
   template std::string CStreamReader::readAt(Relative);
   template double      CStreamReader::readAt(Relative);
   template uint8_t     CStreamReader::readAt(Relative);
   template uint16_t    CStreamReader::readAt(Relative);
   template uint32_t    CStreamReader::readAt(Relative);
   template int8_t      CStreamReader::readAt(Relative);
   template int16_t     CStreamReader::readAt(Relative);
   template int32_t     CStreamReader::readAt(Relative);
   
   std::vector<uint8_t> CStreamReader::read(size_t bytes)
   {
      if (bytes == 0)
      {  return std::vector<uint8_t>(); }
      
      std::vector<uint8_t> buffer(bytes, 0);
      m_raw->read((char*)&buffer[0], bytes);
      
      return buffer;
   }
   
   void CStreamReader::moveTo(Absolute position)
   {
      std::streampos p(position.position());
      if (std::abs(p) > static_cast<std::streampos>(getSize()))
      {  throw std::range_error("Position to move to is beyond file size"); }
      
      auto const from(p >= 0 ? std::ios_base::beg : std::ios_base::end);
      m_raw->seekg(p, from);
   }
   
   void CStreamReader::moveTo(Relative offset) 
   {  moveTo(getPosition() + offset); }
   
   void CStreamReader::moveToBegin() 
   {  m_raw->seekg(0, std::ios_base::beg); }
   
   void CStreamReader::moveToEnd() 
   {  m_raw->seekg(0, std::ios_base::end); }
   
   boost::filesystem::path CStreamReader::getPath() const
   {  return m_path; }
   
   Absolute CStreamReader::getPosition() const
   {  return Absolute(m_raw->tellg()); }
   
   size_t CStreamReader::getSize() const 
   {  return m_size; }
   
   void CStreamReader::setByteOrder(ByteOrder byteOrder) 
   {  m_streamOrder = byteOrder; }
   
   CStreamReader::Token CStreamReader::setByteOrderTemporarily(ByteOrder byteOrder)
   {
      auto b = m_streamOrder;
      m_streamOrder = byteOrder;
      return Token([this, b]{ m_streamOrder = b; });
   }
   
   std::ostream& operator<<(std::ostream& os, CStreamReader const& reader)
   {  return os << "Position: " << reader.getPosition(); }
}}
