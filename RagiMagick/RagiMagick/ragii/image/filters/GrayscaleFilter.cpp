#include <array>
#include <iostream>
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif
#include "GrayscaleFilter.h"

using namespace std;
using namespace ragii::image;

namespace {
	constexpr float WEIGHT_B = 0.298912f;
	constexpr float WEIGHT_G = 0.586611f;
	constexpr float WEIGHT_R = 0.114478f;
}

void GrayscaleFilter::apply()
{
	int w = m_Params.width;
	int h = m_Params.height;
	int d = m_Params.bitCount / 8;
	uint8_t* img = m_Params.image;

	if (d != 4)
	{
		cout << "depth " << d << " not supported." << endl;
		return;
	}

	// 係数
	__m128 weight_b = _mm_set_ps1(WEIGHT_B);
	__m128 weight_g = _mm_set_ps1(WEIGHT_G);
	__m128 weight_r = _mm_set_ps1(WEIGHT_R);
	__m128 weight_a = _mm_set_ps1(0.0f);

	// 配置先
	__m128i mapping_b = _mm_setr_epi8(0, -1, -1, -1, 4, -1, -1, -1,  8, -1, -1, -1, 12, -1, -1, -1);
	__m128i mapping_g = _mm_setr_epi8(1, -1, -1, -1, 5, -1, -1, -1,  9, -1, -1, -1, 13, -1, -1, -1);
	__m128i mapping_r = _mm_setr_epi8(2, -1, -1, -1, 6, -1, -1, -1, 10, -1, -1, -1, 14, -1, -1, -1);
	__m128i mapping_a = _mm_setr_epi8(3, -1, -1, -1, 7, -1, -1, -1, 11, -1, -1, -1, 15, -1, -1, -1);
	__m128i mapping_result = _mm_setr_epi8(0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1);

	for (int i = 0; i < w * h; i += 4)
	{
		// TODO: グレースケールにはなったけど・・・なぜがモザイクが掛かったようになってしまった。直す。

		// 4px (4bytes/px * 4) ロード
		auto src = _mm_load_si128(reinterpret_cast<__m128i*>(img));

		// 並べ替え
		auto shuffle_b = _mm_shuffle_epi8(src, mapping_b);
		auto shuffle_g = _mm_shuffle_epi8(src, mapping_g);
		auto shuffle_r = _mm_shuffle_epi8(src, mapping_r);
		auto shuffle_a = _mm_shuffle_epi8(src, mapping_a);

		// float化(係数の型に合わせる)
		auto s_b = _mm_cvtepi32_ps(shuffle_b);
		auto s_g = _mm_cvtepi32_ps(shuffle_g);
		auto s_r = _mm_cvtepi32_ps(shuffle_r);
		auto s_a = _mm_cvtepi32_ps(shuffle_a);

		// 乗算
		auto m_b = _mm_mul_ps(s_b, weight_b);
		auto m_g = _mm_mul_ps(s_g, weight_g);
		auto m_r = _mm_mul_ps(s_r, weight_r);
		auto m_a = _mm_mul_ps(s_a, weight_a);

		// 加算
		auto s_gray = _mm_add_ps(_mm_add_ps(m_b, m_g), m_r);

		// uint8化(格納する型に合わせる)
		auto i_gray = _mm_cvtps_epi32(s_gray);

		// 並べ替え
		auto shuffle_result = _mm_shuffle_epi8(i_gray, mapping_result);

		// 格納
		_mm_store_si128(reinterpret_cast<__m128i*>(img), shuffle_result);
		

		img += 16;
	}
}
