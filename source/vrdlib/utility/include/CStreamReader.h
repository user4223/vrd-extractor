#pragma once

#include "PositionUtility.h"

#include <filesystem>

#include <iosfwd>
#include <vector>
#include <functional>
#include <memory>

namespace VRD
{
   namespace Utility
   {
      enum class ByteOrder
      {
         LittleEndian,
         BigEndian
      };

      std::ostream &operator<<(std::ostream &os, ByteOrder order);

      struct CStreamReader
      {
         CStreamReader(std::filesystem::path const &path, ByteOrder byteOrder = ByteOrder::LittleEndian);

         template <typename T>
         T read();
         template <typename T>
         T readRaw(); ///< Without any possible turn for endianess
         std::vector<uint8_t> read(size_t bytes);

         template <typename T>
         T readAt(Absolute position);
         template <typename T>
         T readAt(Relative offset);

         void moveTo(Absolute position);
         void moveTo(Relative offset);
         void moveToBegin();
         void moveToEnd();

         std::filesystem::path getPath() const;
         Absolute getPosition() const;
         size_t getSize() const;

         void setByteOrder(ByteOrder byteOrder);

         struct Token
         {
            Token(std::function<void()> f) : m_reset(f) {}
            ~Token()
            {
               if (m_reset)
               {
                  m_reset();
               }
            }
            Token(Token const &) = delete;
            Token(Token &&) = default;
            Token &operator=(Token const &) = delete;
            Token &operator=(Token &&) = default;

         private:
            std::function<void()> m_reset;
         };
         /** \return token until alive, the byte order as given as
          *          a parameter is enabled.
          */
         Token setByteOrderTemporarily(ByteOrder byteOrder);

      private:
         std::filesystem::path m_path;
         std::unique_ptr<std::istream> m_raw;
         ByteOrder m_streamOrder;
         ByteOrder m_hostOrder;
         size_t m_size;
      };

      std::ostream &operator<<(std::ostream &os, CStreamReader const &reader);
   }
}
