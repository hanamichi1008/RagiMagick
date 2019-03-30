#include <algorithm>
#include <array>
#include <cmath>
#include "common.h"
#include "MosaicFilter.h"

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

    inline void mosaic_default(uint8_t* img, int width, int height, int depth, int blockWidth = 20)
    {
        int row, col = 0;
        Color tempColor = {};
        Color resultColor = {};
        const int pxPerBlock = blockWidth * blockWidth;

        for (row = 0; row < height - blockWidth; row += blockWidth) {
            for (col = 0; col < width * depth; col += (blockWidth * depth)) {
                resultColor = {};

                for (int block_y = 0; block_y < blockWidth; block_y++) {
                    for (int block_x = 0; block_x < blockWidth; block_x++) {
                        int rowOffset = block_y;
                        int colOffset = block_x * depth;
                        tempColor = getColor(img, width, depth, row + rowOffset, col + colOffset);
                        resultColor.b += tempColor.b;
                        resultColor.g += tempColor.g;
                        resultColor.r += tempColor.r;
                    }
                }

                resultColor.b = std::clamp(resultColor.b / pxPerBlock, 0x00, 0xff);
                resultColor.g = std::clamp(resultColor.g / pxPerBlock, 0x00, 0xff);
                resultColor.r = std::clamp(resultColor.r / pxPerBlock, 0x00, 0xff);

                for (int block_y = 0; block_y < blockWidth; block_y++) {
                    for (int block_x = 0; block_x < blockWidth; block_x++) {
                        int rowOffset = block_y;
                        int colOffset = block_x * depth;
                        setColor(img, width, depth, row + rowOffset, col + colOffset, resultColor);
                    }
                }
            }
        }
    }

}  // namespace

FilterInfo MosaicFilter::apply(const FilterInfo& info)
{
    mosaic_default(info.image.get(), info.width, info.height, info.bitCount / 8, 50);

    return FilterInfo { info.width, info.height, info.bitCount, info.image };
}
