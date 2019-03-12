#include "LaplacianFilter.h"

#include <memory>

using namespace ragii::image;


namespace {

	// TODO: move対応
	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
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

	void setPixels(uint8_t* img, int width, int depth, int row, int col, const Color& color)
	{
		*(img + (row * width * depth + col + 0)) = color.b;
		*(img + (row * width * depth + col + 1)) = color.g;
		*(img + (row * width * depth + col + 2)) = color.r;
	}

}

void LaplacianFilter::apply()
{
	int w = m_Params.width;
	int h = m_Params.height;
	int d = 24 / 8; // TODO:
	uint8_t* img = m_Params.image;

	int coef[] =
	{
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};

	int rowOffsets[] =
	{
		-1, -1, -1,
		 0,  0,  0,
		 1,  1,  1
	};

	int colOffsets[] = 
	{
		-3,  0,  3,
		-3,  0,  3,
		-3,  0,  3
	};

	int row, col = 0;
	int i = 0;
	Color color = {};

	for (row = 1; row < h - 1; row++)
	{
		for (col = d; col < w * d - d; col += d)
		{
			for (i = 0; i < 9; i++)
			{
				color = getColor(img, w, d, row + rowOffsets[i], col + colOffsets[i]);
				color.b *= coef[i];
				color.g *= coef[i];
				color.r *= coef[i];
			}

			setPixels(img, w, d, row, col, color);
		}
	}


}
