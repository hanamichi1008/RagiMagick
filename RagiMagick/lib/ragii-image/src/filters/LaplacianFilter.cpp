#include <algorithm>
#include <iostream>
#include <memory>
#include "LaplacianFilter.h"

using namespace std;
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

}  // namespace

void LaplacianFilter::apply()
{
    int w = m_Params.width;
    int h = m_Params.height;
    int d = m_Params.bitCount / 8;
    uint8_t* img = m_Params.image;

    if (d != 3 && d != 4) {
        cout << "depth " << d << " not supported." << endl;
        return;
    }

    // 係数
    int coef[] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };
    // 原画像(デバッグ時確認用)
    // int coef[] = { 0, 0, 0, 0, 1, 0, 0, 0, 0 };

    // 基準ピクセルからの行オフセット
    int rowOffsets[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
    // 基準ピクセルからの列オフセット
    int bgrColOffsets[] = { -3, 0, 3, -3, 0, 3, -3, 0, 3 };
    // 基準ピクセルからの列オフセット
    int bgraColOffsets[] = { -4, 0, 4, -4, 0, 4, -4, 0, 4 };

    const int* colOffsets = d == 3 ? bgrColOffsets : bgraColOffsets;

    int row, col = 0;
    int i = 0;
    Color tempColor = {};
    Color resultColor = {};

    for (row = 1; row < h - 1; row++) {
        for (col = d; col < w * d - d; col += d) {
            resultColor = {};

            for (i = 0; i < 9; i++) {
                tempColor = getColor(img, w, d, row + rowOffsets[i], col + colOffsets[i]);
                resultColor.b += (tempColor.b * coef[i]);
                resultColor.g += (tempColor.g * coef[i]);
                resultColor.r += (tempColor.r * coef[i]);
            }

            resultColor.b = std::clamp(resultColor.b, 0x00, 0xff);
            resultColor.g = std::clamp(resultColor.g, 0x00, 0xff);
            resultColor.r = std::clamp(resultColor.r, 0x00, 0xff);

            setColor(img, w, d, row, col, resultColor);
        }
    }
}
