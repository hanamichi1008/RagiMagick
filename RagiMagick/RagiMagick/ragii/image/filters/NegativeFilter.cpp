#include "NegativeFilter.h"


using namespace ragii::image;


void NegativeFilter::apply()
{
	int w = m_Params.width;
	int h = m_Params.height;
	int d = 24 / 8; // TODO:
	uint8_t* img = m_Params.image;

	for (int i = 0; i < w * h * d; i += d)
	{
		*(img + i + 0) = 0xff - *(img + i + 0);
		*(img + i + 1) = 0xff - *(img + i + 1);
		*(img + i + 2) = 0xff - *(img + i + 2);
	}

}
