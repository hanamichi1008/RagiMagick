#include "BitmapConverter.h"
#include "Bitmap.h"


void BitmapConverter::negative(const Bitmap* src, Bitmap* dst)
{
	auto src_data = src->getData().get();
	auto dst_data = dst->getData().get();

	int w = src->getWidth();
	int h = src->getHeight();
	int d = src->getBitCount() / 8;

	for (int i = 0; i < w * h * d; i += d)
	{
		if (d != 3)
		{
			continue;
		}

		uint8_t r = 0xff - *(src_data + i + 0);
		uint8_t g = 0xff - *(src_data + i + 1);
		uint8_t b = 0xff - *(src_data + i + 2);

		*(dst_data + i + 0) = r;
		*(dst_data + i + 1) = g;
		*(dst_data + i + 2) = b;
	}
}
