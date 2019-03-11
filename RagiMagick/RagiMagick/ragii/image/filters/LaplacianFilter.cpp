#include "LaplacianFilter.h"

#include <memory>
#include <tuple>

using namespace ragii::image;


static std::tuple<uint8_t, uint8_t, uint8_t> getPixels(uint8_t* img, int width, int row, int col)
{
	return
		std::make_tuple(
			*(img + (row * width + col + 0)),
			*(img + (row * width + col + 1)),
			*(img + (row * width + col + 2))
		);
}

static void setPixels(uint8_t* img, int width, int row, int col, std::tuple<uint8_t, uint8_t, uint8_t> pixels)
{
	*(img + (row * width + col + 0)) = std::get<0>(pixels);
	*(img + (row * width + col + 1)) = std::get<1>(pixels);
	*(img + (row * width + col + 2)) = std::get<2>(pixels);
}

// TODO: 過去に勢いで実装したせいで、何やってるかわからん。見直す。
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

	int row, col;
	int result = 0;
	int i;

	for (row = 1; row < h - 1; row++)
	{
		for (col = d; col < w * d - d; col += d)
		{
			result = 0;

			for (i = 0; i < 9; i++)
			{
				auto rgb = getPixels(img, w, row + rowOffsets[i], col + colOffsets[i]);
				result += (std::get<0>(rgb) * coef[i] + std::get<1>(rgb) * coef[i] + std::get<2>(rgb) * coef[i]);
			}

			if (result < 0)
			{
				result = 0;
			}

			if (result > 0xff)
			{
				result = 0xff;
			}

			setPixels(img, w, row, col, std::make_tuple(result, result, result));

		}
	}


}
