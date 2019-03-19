#include <array>
#include <iostream>
#include "common.h"
#include "MosaicFilter.h"

using namespace std;
using namespace ragii::image;

namespace {

    const int SmallBlockSize = 3 * 3;
    const int LargeBlockSize = 5 * 5;

    // 基準ピクセルからの行オフセット(3x3)
    constexpr array<int, SmallBlockSize> SmallBlockRowOffsets = {
        -1, -1, -1,
         0,  0,  0,
         1,  1,  1
    };

    // 基準ピクセルからの行オフセット(5x5)
    constexpr array<int, LargeBlockSize> LargeBlockRowOffsets = {
        -2, -2, -2, -2, -2,
        -1, -1, -1, -1, -1,
         0,  0,  0,  0,  0,
         1,  1,  1,  1,  1,
         2,  2,  2,  2,  2
    };

    // 基準ピクセルからの列オフセット(3x3)
    constexpr array<int, SmallBlockSize> SmallBlockColOffsetsBGR = {
        -3, 0, 3,
        -3, 0, 3,
        -3, 0, 3
    };

    // 基準ピクセルからの列オフセット(3x3)
    constexpr array<int, SmallBlockSize> SmallBlockColOffsetsBGRA = {
        -4, 0, 4,
        -4, 0, 4,
        -4, 0, 4
    };

    // 基準ピクセルからの列オフセット(5x5)
    constexpr array<int, LargeBlockSize> LargeBlockColOffsetsBGR = {
        -6, -3, 0, 3, 6,
        -6, -3, 0, 3, 6,
        -6, -3, 0, 3, 6,
        -6, -3, 0, 3, 6,
        -6, -3, 0, 3, 6
    };

    // 基準ピクセルからの列オフセット(5x5)
    constexpr array<int, LargeBlockSize> LargeBlockColOffsetsBGRA = {
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

    Color getColor(uint8_t* img, int width, int depth, int row, int col)
    {
        return
            Color {
                *(img + (row * width * depth + col + 0)),
                *(img + (row * width * depth + col + 1)),
                *(img + (row * width * depth + col + 2))
            };
    }

    void setColor(uint8_t* img, int width, int depth, int row, int col, const Color& color)
    {
        *(img + (row * width * depth + col + 0)) = color.b;
        *(img + (row * width * depth + col + 1)) = color.g;
        *(img + (row * width * depth + col + 2)) = color.r;
    }

}

void MosaicFilter::apply()
{
    int w = m_Params.width;
    int h = m_Params.height;
    int d = m_Params.bitCount / 8;
    uint8_t* img = m_Params.image;

    if (/*d != 3 && */d != 4)
    {
        cout << "depth " << d << " not supported." << endl;
        return;
    }

    int row, col = 0;
    int i = 0;
    Color tempColor = {};
    Color resultColor = {};

// TODO: 何かで判断して実行時に切り替えたい
#define USE_SMALL_BLOCK 0
#if USE_SMALL_BLOCK
    int blockSize = SmallBlockSize;
    const auto& rowOffsets = SmallBlockRowOffsets;
    const auto& colOffsets = d == 3 ? SmallBlockColOffsetsBGR : SmallBlockColOffsetsBGRA;
#else
    int blockSize = LargeBlockSize;
    const auto& rowOffsets = LargeBlockRowOffsets;
    const auto& colOffsets = d == 3 ? LargeBlockColOffsetsBGR : LargeBlockColOffsetsBGRA;
#endif

    // TODO: 何も考えずに書いたから全部見直す。
    for (row = 1; row < h - 1; row++)
    {
        for (col = d; col < w * d - d; col += d)
        {
            resultColor = {};

            for (i = 0; i < blockSize; i++)
            {
                tempColor = std::move(getColor(img, w, d, row + rowOffsets[i], col + colOffsets[i]));
                resultColor.b += tempColor.b;
                resultColor.g += tempColor.g;
                resultColor.r += tempColor.r;
            }

            resultColor.b = std::clamp(resultColor.b / blockSize, 0x00, 0xff);
            resultColor.g = std::clamp(resultColor.g / blockSize, 0x00, 0xff);
            resultColor.r = std::clamp(resultColor.r / blockSize, 0x00, 0xff);

            setColor(img, w, d, row, col, resultColor);
        }
    }
}
