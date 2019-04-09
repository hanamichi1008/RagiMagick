#include "common.h"

namespace ragii::image
{
    uint8_t search_average(uint8_t* img, int w, int h, int d);
    void binary_normal(uint8_t* img, int w, int h, int d);
}  // namespace ragii::image

namespace ragii::image::simd
{
    void binary_sse42(uint8_t* img, int w, int h, int d);
    void binary_avx2(uint8_t* img, int w, int h, int d);
}  // namespace ragii::image::simd
