#include "common.h"
#include "NegativeFilter.h"

using namespace std;
using namespace ragii::image;

namespace
{
    inline void negative_default(uint8_t* img, int width, int height)
    {
        __m128i src_a, src_b, dst;
        src_b = _mm_set1_epi8(0x7f);

        // 一度に 4px (BGRA 32bit * 4 = 128 bit) 処理する
        for (int i = 0; i < width * height; i += 4) {
            src_a = _mm_load_si128(reinterpret_cast<__m128i*>(img));
            dst = _mm_sub_epi16(src_b, src_a);
            _mm_store_si128(reinterpret_cast<__m128i*>(img), dst);
            img += 16;
        }
    }
}

FilterInfo NegativeFilter::apply(const FilterInfo& info)
{
    negative_default(info.image.get(), info.width, info.height);

    return FilterInfo { info.width, info.height, info.bitCount, info.image };
}
