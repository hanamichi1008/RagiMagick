#pragma once

#include <cstdint>

namespace ragii
{
    namespace image
    {
#pragma pack(push, 2)

        struct BitmapInfoHeader
        {
            uint32_t Size;
            int32_t Width;
            int32_t Height;
            uint16_t Planes;
            uint16_t BitCount;
            uint32_t Compression;
            uint32_t SizeImage;
            int32_t XPelsPerMeter;
            int32_t YPelsPerMeter;
            uint32_t ClrUsed;
            uint32_t ClrImportant;
        };

#pragma pack(pop)

        constexpr uint32_t BI_RGB = 0L;
        constexpr uint32_t BI_RLE8 = 1L;
        constexpr uint32_t BI_RLE4 = 2L;
        constexpr uint32_t BI_BITFIELDS = 3L;
        constexpr uint32_t BI_JPEG = 4L;
        constexpr uint32_t BI_PNG = 5L;

        constexpr size_t BitmapInfoHeaderSize = sizeof(BitmapInfoHeader);

    }  // namespace image
}  // namespace ragii
