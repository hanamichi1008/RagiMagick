#include <iostream>
#include "common.h"
#include "BinaryFilter.h"

using namespace std;
using namespace ragii::image;

namespace
{
	const int THRESHOLD = 0x7f;

	inline void binary_normal(uint8_t* img, int w, int h, int d)
	{
		if (d != 3 && d != 4)
		{
			cout << "depth " << d << " not supported." << endl;
			return;
		}

		for (int i = 0; i < w * h * d; i += d)
		{
			if (img[0] > THRESHOLD ||
				img[1] > THRESHOLD ||
				img[2] > THRESHOLD)
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

	void binary_sse42(uint8_t* img, int w, int h, int d)
	{
		if (d != 4)
		{
			cout << "depth " << d << " not supported." << endl;
			return;
		}

		for (int i = 0; i < w * h; i += 4)
		{
			auto a = _mm_load_si128(reinterpret_cast<__m128i*>(img));
			auto b = _mm_set1_epi32(THRESHOLD);
			auto c = _mm_comgt_epi32(a, b);
			_mm_store_si128(reinterpret_cast<__m128i*>(img), c);
			img += 16;
		}
	}

	void binary_avx(uint8_t* img, int w, int h, int d)
	{
		if (d != 4)
		{
			cout << "depth " << d << " not supported." << endl;
			return;
		}

		for (int i = 0; i < w * h; i += 8)
		{
			// ロード (32bit * 8px = 256bit)
			auto a = _mm256_load_si256(reinterpret_cast<__m256i*>(img));

			// 閾値 (8bit単位)
			auto b = _mm256_set1_epi8(THRESHOLD);

			// c[i] = a[i] > b[i] ? 0xff : 0x00
			auto c = _mm256_cmpgt_epi8(a, b);

			// d = (c[i+0] + c[i+1] + c[i+2] + c[i+3]) * 8
			auto d = _mm256_hadd_epi32(c, c);

			// e[i] = d[i] > 0x00 ? 0xff : 0x00;
			auto e = _mm256_cmpgt_epi8(d, _mm256_setzero_si256());

			// 格納
			_mm256_store_si256(reinterpret_cast<__m256i*>(img), e);

			img += 32;
		}
	}
}

void BinaryFilter::apply()
{
	int w = m_Params.width;
	int h = m_Params.height;
	int d = m_Params.bitCount / 8;
	uint8_t* img = m_Params.image;

	// binary_normal(img, w, h, d);
	// binary_sse42(img, w, h, d); // TODO: "Illegal instruction: 4" ... orz
	binary_avx(img, w, h, d);
}
