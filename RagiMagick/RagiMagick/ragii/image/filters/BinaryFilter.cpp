#include <iostream>
#include "BinaryFilter.h"

using namespace std;
using namespace ragii::image;

void BinaryFilter::apply()
{
	int w = m_Params.width;
	int h = m_Params.height;
	int d = m_Params.bitCount / 8;
	uint8_t* img = m_Params.image;
	const int threshold = 0x7f;

	if (d != 3 && d != 4) {
		cout << "depth " << d << " not supported." << endl;
		return;
	}

	for (int i = 0; i < w * h * d; i += d)
	{
		if (img[0] > threshold ||
			img[1] > threshold ||
			img[2] > threshold)
		{
			img[0] = img[1] = img[2] = 0xff;
		}
		else
		{
			img[0] = img[1] = img[2] = 0x00;
		}

		img += d;
	}
}
