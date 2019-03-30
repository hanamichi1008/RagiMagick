#include <array>
#include <algorithm>
#include <cassert>
#include <memory>
#include "memory/memory.h"
#include "LaplacianFilter.h"

using namespace std;
using namespace ragii;
using namespace ragii::image;

namespace
{
    struct Color
    {
        int b;
        int g;
        int r;
    };

    Color getColor(const uint8_t* img, int width, int depth, int row, int col)
    {
        return Color { *(img + (row * width * depth + col + 0)), *(img + (row * width * depth + col + 1)),
                       *(img + (row * width * depth + col + 2)) };
    }

    void setColor(uint8_t* img, int width, int depth, int row, int col, const Color& color)
    {
        *(img + (row * width * depth + col + 0)) = static_cast<uint8_t>(color.b);
        *(img + (row * width * depth + col + 1)) = static_cast<uint8_t>(color.g);
        *(img + (row * width * depth + col + 2)) = static_cast<uint8_t>(color.r);
    }

    // clang-format off
    constexpr array<int, 9> Kernel = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };

    // clang-format off
    constexpr array<int, 9> DebugKernel = {
        0, 0, 0,
        0, 1, 0,
        0, 0, 0
    };

    inline void laplacian_default(const uint8_t* src, int width, int height, int depth, uint8_t* dst)
    {
        assert(src);
        assert(dst);

        // 基準ピクセルからの行オフセット
        int rowOffsets[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
        // 基準ピクセルからの列オフセット
        int colOffsets[] = { -4, 0, 4, -4, 0, 4, -4, 0, 4 };

        int row, col = 0;
        int i = 0;
        Color tempColor = {};
        Color resultColor = {};

        for (row = 1; row < height - 1; row++) {
            for (col = depth; col < width * depth - depth; col += depth) {
                resultColor = {};

                for (i = 0; i < Kernel.size(); i++) {
                    tempColor = getColor(src, width, depth, row + rowOffsets[i], col + colOffsets[i]);
                    resultColor.b += (tempColor.b * Kernel[i]);
                    resultColor.g += (tempColor.g * Kernel[i]);
                    resultColor.r += (tempColor.r * Kernel[i]);
                }

                resultColor.b = std::clamp(resultColor.b, 0x00, 0xff);
                resultColor.g = std::clamp(resultColor.g, 0x00, 0xff);
                resultColor.r = std::clamp(resultColor.r, 0x00, 0xff);

                setColor(dst, width, depth, row, col, resultColor);
            }
        }
    }

}  // namespace

FilterInfo LaplacianFilter::apply(const FilterInfo& info)
{
    auto src = info.image;
    auto dst = aligned_allocator<uint8_t>::make_shared(info.width * info.height * info.bitCount / 8, 16);

    laplacian_default(src.get(), info.width, info.height, info.bitCount / 8, dst.get());


    return FilterInfo { info.width, info.height, info.bitCount, dst };
}
