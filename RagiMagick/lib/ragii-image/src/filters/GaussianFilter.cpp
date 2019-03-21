#include <array>
#include "GaussianFilter.h"
#include <iostream>
#include <algorithm>
#include "common.h"

using namespace std;
using namespace ragii::image;

namespace
{
const int SmallKernelSize = 9;
const int LargeKernelSize = 25;

constexpr array<float, SmallKernelSize> SmallKernel = {
    1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f, 2.0f / 16.0f, 4.0f / 16.0f,
    2.0f / 16.0f, 1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f};

constexpr array<float, LargeKernelSize> LargeKernel = {
    1.0f / 256.0f,  4.0f / 256.0f,  6.0f / 256.0f,  4.0f / 256.0f,
    1.0f / 256.0f,  4.0f / 256.0f,  16.0f / 256.0f, 24.0f / 256.0f,
    16.0f / 256.0f, 4.0f / 256.0f,  6.0f / 256.0f,  24.0f / 256.0f,
    36.0f / 256.0f, 24.0f / 256.0f, 6.0f / 256.0f,  4.0f / 256.0f,
    16.0f / 256.0f, 24.0f / 256.0f, 16.0f / 256.0f, 4.0f / 256.0f,
    1.0f / 256.0f,  4.0f / 256.0f,  6.0f / 256.0f,  4.0f / 256.0f,
    1.0f / 256.0f,
};

// 基準ピクセルからの行オフセット(3x3)
constexpr array<int, SmallKernelSize> SmallKernelRowOffsets = {-1, -1, -1, 0, 0,
                                                               0,  1,  1,  1};

// 基準ピクセルからの行オフセット(5x5)
constexpr array<int, LargeKernelSize> LargeKernelRowOffsets = {
    -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0,
    0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  2, 2};

// 基準ピクセルからの列オフセット(3x3)
constexpr array<int, SmallKernelSize> SmallKernelColOffsetsBGR = {
    -3, 0, 3, -3, 0, 3, -3, 0, 3};

// 基準ピクセルからの列オフセット(3x3)
constexpr array<int, SmallKernelSize> SmallKernelColOffsetsBGRA = {
    -4, 0, 4, -4, 0, 4, -4, 0, 4};

// 基準ピクセルからの列オフセット(5x5)
constexpr array<int, LargeKernelSize> LargeKernelColOffsetsBGR = {
    -6, -3, 0,  3,  6, -6, -3, 0,  3,  6, -6, -3, 0,
    3,  6,  -6, -3, 0, 3,  6,  -6, -3, 0, 3,  6};

// 基準ピクセルからの列オフセット(5x5)
constexpr array<int, LargeKernelSize> LargeKernelColOffsetsBGRA = {
    -8, -4, 0,  4,  8, -8, -4, 0,  4,  8, -8, -4, 0,
    4,  8,  -8, -4, 0, 4,  8,  -8, -4, 0, 4,  8};

struct Color {
    int b;
    int g;
    int r;
};

Color getColor(const uint8_t* img, int width, int depth, int row, int col)
{
    return Color{*(img + (row * width * depth + col + 0)),
                 *(img + (row * width * depth + col + 1)),
                 *(img + (row * width * depth + col + 2))};
}

void setColor(uint8_t* img, int width, int depth, int row, int col,
              const Color& color)
{
    *(img + (row * width * depth + col + 0)) = static_cast<uint8_t>(color.b);
    *(img + (row * width * depth + col + 1)) = static_cast<uint8_t>(color.g);
    *(img + (row * width * depth + col + 2)) = static_cast<uint8_t>(color.r);
}

}  // namespace

void GaussianFilter::apply()
{
    int w = m_Params.width;
    int h = m_Params.height;
    int d = m_Params.bitCount / 8;
    uint8_t* img = m_Params.image;

    if (/*d != 3 && */ d != 4) {
        cout << "depth " << d << " not supported." << endl;
        return;
    }

    int row, col = 0;
    size_t i = 0;
    Color tempColor = {};
    Color resultColor = {};

// TODO: 何かで判断して実行時に切り替えたい
#define USE_SMALL_KERNEL 0
#if USE_SMALL_KERNEL
    const auto& kernel = SmallKernel;
    const auto& rowOffsets = SmallKernelRowOffsets;
    const auto& colOffsets =
        d == 3 ? SmallKernelColOffsetsBGR : SmallKernelColOffsetsBGRA;
#else
    const auto& kernel = LargeKernel;
    const auto& rowOffsets = LargeKernelRowOffsets;
    const auto& colOffsets =
        d == 3 ? LargeKernelColOffsetsBGR : LargeKernelColOffsetsBGRA;
#endif

    for (row = 1; row < h - 1; row++) {
        for (col = d; col < w * d - d; col += d) {
            resultColor = {};

            for (i = 0; i < kernel.size(); i++) {
                tempColor = getColor(img, w, d, row + rowOffsets[i],
                                     col + colOffsets[i]);
                resultColor.b += static_cast<int>(tempColor.b * kernel[i]);
                resultColor.g += static_cast<int>(tempColor.g * kernel[i]);
                resultColor.r += static_cast<int>(tempColor.r * kernel[i]);
            }

            resultColor.b = std::clamp(resultColor.b, 0x00, 0xff);
            resultColor.g = std::clamp(resultColor.g, 0x00, 0xff);
            resultColor.r = std::clamp(resultColor.r, 0x00, 0xff);

            setColor(img, w, d, row, col, resultColor);
        }
    }
}
