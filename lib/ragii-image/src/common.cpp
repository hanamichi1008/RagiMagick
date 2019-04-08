#include "common.h"

using namespace std;
using namespace ragii;
using namespace ragii::hardware;

namespace ragii::image
{
    unique_aligned_ptr bgr24_to_bgra32_default(uint8_t* src, int width, int height)
    {
        auto dst = aligned_allocator<uint8_t>::make_unique(width * height * 4, 16);
        auto p = dst.get();

#if defined(__clang__)
#    pragma clang loop vectorize(disable)
#elif defined(_MSC_VER)
#    pragma loop(no_vector)
#endif
        for (int i = 0; i < width * height * 3; i += 3) {
            p[0] = src[i + 0];
            p[1] = src[i + 1];
            p[2] = src[i + 2];
            p[3] = 0;
            p += 4;
        }

        return dst;
    }

    unique_aligned_ptr bgr24_to_bgra32(uint8_t* src, int width, int height)
    {
        unique_aligned_ptr dst;

        if (CpuInfo::avx2()) {
            // TODO: avx2
            dst = bgr24_to_bgra32_default(src, width, height);
        }
        else if (CpuInfo::avx()) {
            // TODO: avx
            dst = bgr24_to_bgra32_default(src, width, height);
        }
        else if (CpuInfo::sse42()) {
            // TODO: sse
            dst = bgr24_to_bgra32_default(src, width, height);
        }
        else {
            dst = bgr24_to_bgra32_default(src, width, height);
        }

        return dst;
    }
}  // namespace ragii::image
