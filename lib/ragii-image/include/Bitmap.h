#pragma once

#include <memory>
#include <string>
#include "memory/memory.h"
#include "formats/bitmap/BitmapHeader.h"

namespace ragii
{
    namespace image
    {
        class Bitmap
        {
        public:
            using ImagePtr = std::unique_ptr<uint8_t, aligned_deleter<uint8_t>>;

            static std::unique_ptr<Bitmap> loadFromFile(std::string path);
            static std::unique_ptr<Bitmap> create(int32_t width, int32_t height, int16_t bitCount = 24);

            void save(std::string path);

            const BitmapHeader& getHeader() const;

            decltype(auto) getData() { return m_Data.get(); }
            decltype(auto) getData() const { return m_Data.get(); }

            int32_t getWidth() const;
            int32_t getHeight() const;
            uint16_t getBitCount() const;

        private:
            BitmapHeader m_Header = {};
            ImagePtr m_Data;
        };

    }  // namespace image
}  // namespace ragii
