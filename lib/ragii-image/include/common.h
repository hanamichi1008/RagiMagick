#pragma once

// include
#if defined(_MSC_VER)
#    include <intrin.h>
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
#    include <x86intrin.h>
#endif

// warning
#if defined(__clang__)
#    pragma clang diagnostic ignored "-Wunused-const-variable"
#elif defined(__GNUC__) || defined(__GNUG__)
#    pragma GCC diagnostic ignored "-Wunused-const-variable"
#endif

#include <cstdint>
#include <memory>
#include "hardware/cpu_info.h"
#include "memory/memory.h"

namespace ragii::image
{
    using unique_aligned_ptr = std::unique_ptr<uint8_t, aligned_deleter<uint8_t>>;

    inline auto allocate(size_t size)
    {
        return std::make_unique<uint8_t>(size);
    }

    inline auto allocate(size_t size, int alignment)
    {
        return aligned_allocator<uint8_t>::make_unique(size, alignment);
    }

    struct Color
    {
        int b;
        int g;
        int r;
        int a;
    };

    inline Color getColor(const uint8_t* img, int width, int depth, int row, int col)
    {
        int offset = row * width * depth + col;
        Color color = {};

        if (depth == 3) {
            color.b = *(img + (offset + 0));
            color.g = *(img + (offset + 1));
            color.r = *(img + (offset + 2));
        }
        else if (depth == 4) {
            color.b = *(img + (offset + 0));
            color.g = *(img + (offset + 1));
            color.r = *(img + (offset + 2));
            color.a = *(img + (offset + 3));
        }
        else {
            // TODO:
        }

        return color;
    }

    inline void setColor(uint8_t* img, int width, int depth, int row, int col, const Color& color)
    {
        int offset = row * width * depth + col;

        if (depth == 3) {
            *(img + (offset + 0)) = static_cast<uint8_t>(color.b);
            *(img + (offset + 1)) = static_cast<uint8_t>(color.g);
            *(img + (offset + 2)) = static_cast<uint8_t>(color.r);
        }
        else if (depth == 4) {
            *(img + (offset + 0)) = static_cast<uint8_t>(color.b);
            *(img + (offset + 1)) = static_cast<uint8_t>(color.g);
            *(img + (offset + 2)) = static_cast<uint8_t>(color.r);
            *(img + (offset + 3)) = static_cast<uint8_t>(color.a);
        }
        else {
            // TODO:
        }
    }

    unique_aligned_ptr bgr24_to_bgra32_default(uint8_t* src, int width, int height);
    unique_aligned_ptr bgr24_to_bgra32(uint8_t* src, int width, int height);
}  // namespace ragii::image

namespace ragii::image::simd
{
    // 符号"無し" 8ビット整数16個の比較
    inline __m128i cmpgte_epu8(__m128i a, __m128i b)
    {
        return _mm_cmpeq_epi8(_mm_max_epu8(a, b), a);
    }
}  // namespace ragii::image::simd
