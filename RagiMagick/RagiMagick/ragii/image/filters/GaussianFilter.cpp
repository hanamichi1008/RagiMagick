#include <array>
#include <iostream>
#include "common.h"
#include "GaussianFilter.h"

using namespace std;
using namespace ragii::image;

namespace {

    constexpr std::array<float, 9> SmallKernel = {
        1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f,
        2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f,
        1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f
    };

    constexpr std::array<float, 25> LargeKernel = {
        1.0f/256.0f,  4.0f/256.0f,  6.0f/256.0f,  4.0f/256.0f, 1.0f/256.0f,
        4.0f/256.0f, 16.0f/256.0f, 24.0f/256.0f, 16.0f/256.0f, 4.0f/256.0f,
        6.0f/256.0f, 24.0f/256.0f, 36.0f/256.0f, 24.0f/256.0f, 6.0f/256.0f,
        4.0f/256.0f, 16.0f/256.0f, 24.0f/256.0f, 16.0f/256.0f, 4.0f/256.0f,
        1.0f/256.0f,  4.0f/256.0f,  6.0f/256.0f,  4.0f/256.0f, 1.0f/256.0f,
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

void GaussianFilter::apply()
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

	// 基準ピクセルからの行オフセット
	int rowOffsets[] = { -1, -1, -1, 0, 0, 0, 1,  1,  1 };
	// 基準ピクセルからの列オフセット
	int bgrColOffsets[] = { -3, 0, 3, -3, 0, 3, -3, 0, 3 };
	// 基準ピクセルからの列オフセット
	int bgraColOffsets[] = { -4, 0, 4, -4, 0, 4, -4, 0, 4 };

	const int* colOffsets = d == 3 ? bgrColOffsets : bgraColOffsets;

	int row, col = 0;
	int i = 0;
	Color tempColor = {};
	Color resultColor = {};

	for (row = 1; row < h - 1; row++)
	{
		for (col = d; col < w * d - d; col += d)
		{
			resultColor = {};

			for (i = 0; i < SmallKernel.size(); i++)
			{
				tempColor = std::move(getColor(img, w, d, row + rowOffsets[i], col + colOffsets[i]));
				resultColor.b += (tempColor.b * SmallKernel[i]);
				resultColor.g += (tempColor.g * SmallKernel[i]);
				resultColor.r += (tempColor.r * SmallKernel[i]);
			}

			resultColor.b = std::clamp(resultColor.b, 0x00, 0xff);
			resultColor.g = std::clamp(resultColor.g, 0x00, 0xff);
			resultColor.r = std::clamp(resultColor.r, 0x00, 0xff);

			setColor(img, w, d, row, col, resultColor);
		}
	}
}
