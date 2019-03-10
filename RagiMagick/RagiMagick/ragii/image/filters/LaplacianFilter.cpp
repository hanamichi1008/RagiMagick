#include "LaplacianFilter.h"

#include <memory>

using namespace ragii::image;

// TODO: 過去に勢いで実装したせいで、何やってるかわからん。見直す。
void LaplacianFilter::apply()
{
	int w = m_Params.width;
	int h = m_Params.height;
	int depth = 24 / 8; // TODO:
	uint8_t* img = m_Params.image;

	// kernel operator
	int kop[] =
	{
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};

	uint8_t** src_img = new uint8_t*[h];
	for (int row = 0; row < h; row++)
	{
		src_img[row] = &img[w * row * depth];
	}

	uint8_t* edge_img = new uint8_t[w * h * depth];
	int d[9];
	int row, col, px;
	int xx;
	int amp = 10;
	int left, center, right;
	left = center = right = 0;

	for (row = 1; row < h - 1; row++)
	{
		for (col = 1; col < w * depth - 1; col++)
		{
			d[0] = src_img[row - 1][col - 1];
			d[1] = src_img[row - 1][col + 0];
			d[2] = src_img[row - 1][col + 1];
			d[3] = src_img[row + 0][col - 1];
			d[4] = src_img[row + 0][col + 0];
			d[5] = src_img[row + 0][col + 1];
			d[6] = src_img[row + 1][col - 1];
			d[7] = src_img[row + 1][col + 0];
			d[8] = src_img[row + 1][col + 1];
			xx =
				kop[0] * d[0] + kop[1] * d[1] + kop[2] * d[2] +
				kop[3] * d[3] + kop[4] * d[4] + kop[5] * d[5] +
				kop[6] * d[6] + kop[7] * d[7] + kop[8] * d[8];
			px = xx * amp;
			if (px < 0)
			{
				px = -px;
			}
			if (px > 255)
			{
				px = 255;
			}
			*(edge_img + (row * w * depth + col + 0)) = px;
			*(edge_img + (row * w * depth + col + 1)) = px;
			*(edge_img + (row * w * depth + col + 2)) = px;
		}
	}

	memmove(img, edge_img, w * h * depth);
	delete[] edge_img;
	edge_img = nullptr;

	delete[] src_img;
	src_img = nullptr;
}
