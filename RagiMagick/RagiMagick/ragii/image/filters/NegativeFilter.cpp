#include <iostream>
#include "NegativeFilter.h"

using namespace std;
using namespace ragii::image;

void NegativeFilter::apply()
{
	int w = m_Params.width;
	int h = m_Params.height;
	int d = m_Params.bitCount / 8;
	uint8_t* img = m_Params.image;

	if (d != 3 && d != 4) {
		cout << "depth " << d << " not supported." << endl;
		return;
	}

	for (int i = 0; i < w * h * d; i += d)
	{
		img[0] = 0xff - img[0];
		img[1] = 0xff - img[1];
		img[2] = 0xff - img[2];
		img += d;
	}
}
