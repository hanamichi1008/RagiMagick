#include "filters.h"
#include "BinaryFilter.h"
#include "hardware/cpu_info.h"

using namespace std;
using namespace ragii::image;
using namespace ragii::image::simd;
using namespace ragii::hardware;

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
