#include <iostream>
#include "common.h"
#include "BinaryFilter.h"

using namespace std;
using namespace ragii::image;

namespace
{
    const int THRESHOLD = 50;

    inline void binary_normal(uint8_t* img, int w, int h, int d)
    {
        if (d != 3 && d != 4) {
            cout << "depth " << d << " not supported." << endl;
            return;
        }

        for (int i = 0; i < w * h * d; i += d) {
            if (img[0] > THRESHOLD || img[1] > THRESHOLD || img[2] > THRESHOLD) {
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
        if (d != 4) {
            cout << "depth " << d << " not supported." << endl;
            return;
        }

        const __m128i th1 = _mm_set1_epi8(THRESHOLD);  // 閾値 (8bit単位)
        const __m128i th2 = _mm_set1_epi32(0);         // 閾値 (32bit単位)
        __m128i src, dst;

        for (int i = 0; i < w * h; i += 4) {
            // ロード (32bit * 4px = 128bit)
            src = _mm_load_si128(reinterpret_cast<__m128i*>(img));
            // BGR各成分で閾値を超えているかチェック TODO: Aが0前提なのを直す
            dst = _mm_cmpgt_epi8(src, th1);
            // BGRA 全てが 0 なら 0x00000000、そうでなければ 0xffffffff になる
            dst = _mm_cmpgt_epi32(dst, th2);
            _mm_store_si128(reinterpret_cast<__m128i*>(img), dst);

            img += 16;
        }
    }

    void binary_avx(uint8_t* img, int w, int h, int d)
    {
        if (d != 4) {
            cout << "depth " << d << " not supported." << endl;
            return;
        }

        const __m256i th1 = _mm256_set1_epi8(THRESHOLD);  // 閾値 (8bit単位)
        const __m256i th2 = _mm256_set1_epi32(0);         // 閾値 (32bit単位)
        __m256i src, dst;

        for (int i = 0; i < w * h; i += 8) {
            // ロード (32bit * 8px = 256bit)
            src = _mm256_load_si256(reinterpret_cast<__m256i*>(img));
            // BGR各成分で閾値を超えているかチェック TODO: Aが0前提なのを直す
            dst = _mm256_cmpgt_epi8(src, th1);  // TODO: AVX2からだった。自宅の Sandy では Illegal instruction。直す。
            // BGRA 全てが 0 なら 0x00000000、そうでなければ 0xffffffff になる
            dst = _mm256_cmpgt_epi32(dst, th2);
            _mm256_store_si256(reinterpret_cast<__m256i*>(img), dst);

            img += 32;
        }
    }
}  // namespace

void BinaryFilter::apply()
{
    int w = m_Params.width;
    int h = m_Params.height;
    int d = m_Params.bitCount / 8;
    uint8_t* img = m_Params.image;

    // TODO: ちゃんとする
    int mode = 0;
    switch (mode) {
        case 0:
            binary_normal(img, w, h, d);
            break;

        case 1:
            binary_sse42(img, w, h,
                         d);  // TODO: "Illegal instruction: 4" ... orz
            break;

        case 2:
            binary_avx(img, w, h, d);
            break;
    }
}
