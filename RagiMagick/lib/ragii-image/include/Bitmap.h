#pragma once

#include <memory>
#include <string>

#include "formats/bitmap/BitmapHeader.h"

namespace ragii
{
    namespace image
    {
        template<typename T>
        struct aligned_deleter
        {
            void operator()(T* ptr)
            {
                if (ptr) {
                    _aligned_free(ptr);
                    ptr = nullptr;
                }
            }
        };

        template<typename T>
        struct aligned_allocator
        {
            static T* alloc(size_t size, int alignment)
            {
                return reinterpret_cast<T*>(_aligned_malloc(size, alignment));
            }

            static decltype(auto) make_unique(size_t size, int alignment)
            {
                return std::unique_ptr<T, aligned_deleter<T>>(alloc(size, alignment));
            }
        };

        class Bitmap
        {
        public:
            ~Bitmap();
            static std::unique_ptr<Bitmap> loadFromFile(std::string path);
            static std::unique_ptr<Bitmap> create(int32_t width, int32_t height, int16_t bitCount = 24);

            void save(std::string path);

            const BitmapHeader& getHeader() const;

            decltype(auto) getData() { return m_Data; }
            decltype(auto) getData() const { return m_Data; };

            int32_t getWidth() const;
            int32_t getHeight() const;
            int16_t getBitCount() const;

        private:
            BitmapHeader m_Header = {};
            uint8_t* m_Data = nullptr;
        };

    }  // namespace image
}  // namespace ragii
