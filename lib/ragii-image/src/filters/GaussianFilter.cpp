#include <algorithm>
#include <array>
#include <iostream>
#include <cmath>
#include "common.h"
#include "GaussianFilter.h"

using namespace std;
using namespace ragii::image;

namespace
{
    const int SmallKernelSize = 9;
    const int LargeKernelSize = 25;

    // clang-format off
    constexpr array<float, SmallKernelSize> SmallKernel = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };

    // clang-format off
    constexpr array<float, LargeKernelSize> LargeKernel = {
        1.0f / 256.0f,  4.0f / 256.0f,  6.0f / 256.0f,  4.0f / 256.0f, 1.0f / 256.0f,
        4.0f / 256.0f, 16.0f / 256.0f, 24.0f / 256.0f, 16.0f / 256.0f, 4.0f / 256.0f,
        6.0f / 256.0f, 24.0f / 256.0f, 36.0f / 256.0f, 24.0f / 256.0f, 6.0f / 256.0f,
        4.0f / 256.0f, 16.0f / 256.0f, 24.0f / 256.0f, 16.0f / 256.0f, 4.0f / 256.0f,
        1.0f / 256.0f,  4.0f / 256.0f,  6.0f / 256.0f,  4.0f / 256.0f, 1.0f / 256.0f,
    };

    // 基準ピクセルからの行オフセット(3x3)
    // clang-format off
    constexpr array<int, SmallKernelSize> SmallKernelRowOffsets = {
        -1, -1, -1,
         0,  0,  0,
         1,  1,  1
    };

    // 基準ピクセルからの行オフセット(5x5)
    // clang-format off
    constexpr array<int, LargeKernelSize> LargeKernelRowOffsets = {
        -2, -2, -2, -2, -2,
        -1, -1, -1, -1, -1,
         0,  0,  0,  0,  0,
         1,  1,  1,  1,  1,
         2,  2,  2,  2,  2
    };

    // 基準ピクセルからの列オフセット(3x3)
    // clang-format off
    constexpr array<int, SmallKernelSize> SmallKernelColOffsets = {
        -4, 0, 4,
        -4, 0, 4,
        -4, 0, 4
    };

    // 基準ピクセルからの列オフセット(5x5)
    // clang-format off
    constexpr array<int, LargeKernelSize> LargeKernelColOffsets = {
        -8, -4, 0, 4, 8,
        -8, -4, 0, 4, 8,
        -8, -4, 0, 4, 8,
        -8, -4, 0, 4, 8,
        -8, -4, 0, 4, 8
    };

    struct Color
    {
        int b;
        int g;
        int r;
    };

    Color getColor(const uint8_t* img, int width, int depth, int row, int col)
    {
        return Color {
            *(img + (row * width * depth + col + 0)),
            *(img + (row * width * depth + col + 1)),
            *(img + (row * width * depth + col + 2))
        };
    }

    void setColor(uint8_t* img, int width, int depth, int row, int col, const Color& color)
    {
        *(img + (row * width * depth + col + 0)) = static_cast<uint8_t>(color.b);
        *(img + (row * width * depth + col + 1)) = static_cast<uint8_t>(color.g);
        *(img + (row * width * depth + col + 2)) = static_cast<uint8_t>(color.r);
    }

    inline void gaussian_default(uint8_t* img, int width, int height, int depth, bool use_large_kernel)
    {
        int row, col = 0;
        size_t i = 0;
        Color tempColor = {};
        Color resultColor = {};

        int initialRow;
        if (use_large_kernel) {
            initialRow = abs(LargeKernelRowOffsets[0]);
        }
        else {
            initialRow = abs(SmallKernelRowOffsets[0]);
        }

        for (row = initialRow; row < height - initialRow; row++) {
            for (col = depth; col < width * depth - depth; col += depth) {
                resultColor = {};

                if (use_large_kernel) {
                    for (i = 0; i < LargeKernel.size(); i++) {
                        tempColor = getColor(img, width, depth, row + LargeKernelRowOffsets[i], col + LargeKernelColOffsets[i]);
                        resultColor.b += static_cast<int>(tempColor.b * LargeKernel[i]);
                        resultColor.g += static_cast<int>(tempColor.g * LargeKernel[i]);
                        resultColor.r += static_cast<int>(tempColor.r * LargeKernel[i]);
                    }
                }
                else {
                    for (i = 0; i < SmallKernel.size(); i++) {
                        tempColor = getColor(img, width, depth, row + SmallKernelRowOffsets[i], col + SmallKernelColOffsets[i]);
                        resultColor.b += static_cast<int>(tempColor.b * SmallKernel[i]);
                        resultColor.g += static_cast<int>(tempColor.g * SmallKernel[i]);
                        resultColor.r += static_cast<int>(tempColor.r * SmallKernel[i]);
                    }
                }

                resultColor.b = std::clamp(resultColor.b, 0x00, 0xff);
                resultColor.g = std::clamp(resultColor.g, 0x00, 0xff);
                resultColor.r = std::clamp(resultColor.r, 0x00, 0xff);

                setColor(img, width, depth, row, col, resultColor);
            }
        }
    }

}  // namespace

FilterInfo GaussianFilter::apply(const FilterInfo& info)
{
    gaussian_default(info.image.get(), info.width, info.height, info.bitCount / 8, true);

    return FilterInfo { info.width, info.height, info.bitCount, info.image };
}
