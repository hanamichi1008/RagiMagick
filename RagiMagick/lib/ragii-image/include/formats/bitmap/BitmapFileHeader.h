#pragma once

#include <cstdint>

namespace ragii
{
namespace image
{
#pragma pack(push, 2)

struct BitmapFileHeader {
    uint16_t Type;
    uint32_t Size;
    uint16_t Reserved1;
    uint16_t Reserved2;
    uint32_t OffBits;
};

#pragma pack(pop)

constexpr size_t BitmapFileHeaderSize = sizeof(BitmapFileHeader);

}  // namespace image
}  // namespace ragii
