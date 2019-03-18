#include <iostream>
#include "common.h"
#include "NegativeFilter.h"

using namespace std;
using namespace ragii::image;

void NegativeFilter::apply()
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

	__m128i src_a, src_b, dst;
	src_b = _mm_set1_epi8(0xff);

	// 一度に 4px (BGRA 32bit * 4 = 128 bit) 処理する
	for (int i = 0; i < w * h; i += 4)
	{
		src_a = _mm_load_si128(reinterpret_cast<__m128i*>(img));
		dst = _mm_sub_epi16(src_b, src_a);
		_mm_store_si128(reinterpret_cast<__m128i*>(img), dst);
		img += 16;
	}
}
