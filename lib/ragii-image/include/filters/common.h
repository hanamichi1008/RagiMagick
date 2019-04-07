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

#define RAGII_IMAGE_EXTERN_C extern "C"

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

    unique_aligned_ptr bgr24_to_bgra32_default(uint8_t* src, int width, int height);
    unique_aligned_ptr bgr24_to_bgra32(uint8_t* src, int width, int height);
}
