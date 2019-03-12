#include "BinaryFilter.h"


using namespace ragii::image;


void BinaryFilter::apply()
{
	int w = m_Params.width;
	int h = m_Params.height;
	int d = m_Params.bitCount / 8;
	uint8_t* img = m_Params.image;

	const int threshold = 0xaa;
	uint8_t r, g, b, result;

	for (int i = 0; i < w * h * d; i += d)
	{
		r = *(img + i + 0);
		g = *(img + i + 1);
		b = *(img + i + 2);
		
		if (r > threshold || g > threshold || b > threshold)
		{
			result = 0x00;
		}
		else
		{
			result = 0xff;
		}

		*(img + i + 0) = result;
		*(img + i + 1) = result;
		*(img + i + 2) = result;
	}
}
