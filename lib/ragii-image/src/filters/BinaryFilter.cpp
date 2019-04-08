#include <iostream>
#include <numeric>
#include <vector>
#include "common.h"
#include "BinaryFilter.h"
#include "hardware/cpu_info.h"

using namespace std;
using namespace ragii::image;
using namespace ragii::hardware;

namespace
{
    inline uint8_t search_average(uint8_t* img, int w, int h, int d)
    {
        uint8_t result = 0;
        vector<uint8_t> horiz(w);
        vector<uint8_t> vert(h);

        for (size_t y = 0; y < h; y++) {
            for (size_t x = 0, i = 0; x < w * d; i++, x += d) {
                horiz[i] = static_cast<uint8_t>((img[x] + img[x + 1] + img[x + 2]) / 3);
                img += d;
            }
            vert[y] = static_cast<uint8_t>(std::accumulate(horiz.begin(), horiz.end(), 0) / w);
        }

        result = static_cast<uint8_t>(std::accumulate(vert.begin(), vert.end(), 0) / h);

        return result;
    }

    inline void binary_normal(uint8_t* img, int w, int h, int d)
    {
        const uint8_t threshold = search_average(img, w, h, d);

        for (int i = 0; i < w * h * d; i += d) {
            if (img[0] > threshold || img[1] > threshold || img[2] > threshold) {
                img[0] = img[1] = img[2] = 0xff;
            }
            else {
                img[0] = img[1] = img[2] = 0x00;
            }

            img += d;
        }
    }

    void binary_sse42(uint8_t* img, int w, int h, int d)
    {
        const uint8_t threshold = search_average(img, w, h, d);
        const __m128i th1 = _mm_set1_epi8(threshold);  // 閾値 (8bit単位)
        const __m128i th2 = _mm_set1_epi32(0);         // 閾値 (32bit単位)
        __m128i src, dst;

        for (int i = 0; i < w * h; i += 4) {
            // ロード (32bit * 4px = 128bit)
            src = _mm_load_si128(reinterpret_cast<__m128i*>(img));
            // BGR各成分で閾値を超えているかチェック
            dst = simd::cmpgte_epu8(src, th1);
            // BGRA 全てが 0 なら 0x00000000、そうでなければ 0xffffffff になる
            dst = _mm_cmpgt_epi32(dst, th2);
            _mm_store_si128(reinterpret_cast<__m128i*>(img), dst);

            img += 16;
        }
    }

    void binary_avx2(uint8_t* img, int w, int h, int d)
    {
        const uint8_t threshold = search_average(img, w, h, d);
        const __m256i th1 = _mm256_set1_epi8(threshold);  // 閾値 (8bit単位)
        const __m256i th2 = _mm256_set1_epi32(0);         // 閾値 (32bit単位)
        __m256i src, dst;

        for (int i = 0; i < w * h; i += 8) {
            // ロード (32bit * 8px = 256bit)
            src = _mm256_load_si256(reinterpret_cast<__m256i*>(img));
            // BGR各成分で閾値を超えているかチェック
            dst = _mm256_cmpgt_epi8(src, th1);  // これだけが AVX2 以降の命令
            // BGRA 全てが 0 なら 0x00000000、そうでなければ 0xffffffff になる
            dst = _mm256_cmpgt_epi32(dst, th2);
            _mm256_store_si256(reinterpret_cast<__m256i*>(img), dst);

            img += 32;
        }
    }

}  // namespace

FilterInfo BinaryFilter::apply(const FilterInfo& info)
{
    int w = info.width;
    int h = info.height;
    int d = info.bitCount / 8;
    auto img = info.image.get();

    if (CpuInfo::avx2()) {
        binary_avx2(img, w, h, d);
    }
    else if (CpuInfo::avx()) {
        // TODO: avx
        binary_sse42(img, w, h, d);
    }
    else if (CpuInfo::sse42()) {
        binary_sse42(img, w, h, d);
    }
    else {
        binary_normal(img, w, h, d);
    }

    return FilterInfo { info.width, info.height, info.bitCount, info.image };
}
