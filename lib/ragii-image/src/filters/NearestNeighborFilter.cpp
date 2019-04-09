#include <cmath>
#include "common.h"
#include "memory/memory.h"
#include "NearestNeighborFilter.h"

using namespace std;
using namespace ragii::image;

namespace
{
    struct ImageInfo
    {
        uint8_t* image;
        int width;
        int height;
        int depth;
    };

    inline void nn_default(const ImageInfo& src_info, const ImageInfo& dst_info, float ratio)
    {
        if (src_info.depth != dst_info.depth) {
            return;
        }

        auto dst_img = dst_info.image;
        int sx, sy;

        for (int dy = 0; dy < dst_info.height; dy++) {
            sy = static_cast<int>(round(dy / ratio));
            for (int dx = 0; dx < dst_info.width; dx++) {
                sx = static_cast<int>(round(dx / ratio));

                auto color = getColor(src_info.image, src_info.width, src_info.depth, sy, sx * src_info.depth);
                dst_img[0] = color.b;
                dst_img[1] = color.g;
                dst_img[2] = color.r;
                dst_img += dst_info.depth;
            }
        }
    }

}  // namespace

NearestNeighborFilter::NearestNeighborFilter(float ratio)
    : m_Ratio(ratio)
{
}

FilterInfo NearestNeighborFilter::apply(const FilterInfo& info)
{
    int dw = info.width * m_Ratio;
    int dh = info.height * m_Ratio;

    auto src = info.image;
    auto dst = aligned_allocator<uint8_t>::make_shared(dw * dh * info.bitCount / 8, 16);

    if (!dst) {
        return {};
    }

    auto src_info = ImageInfo { src.get(), info.width, info.height, info.bitCount / 8 };
    auto dst_info = ImageInfo { dst.get(), dw, dh, info.bitCount / 8 };

    nn_default(src_info, dst_info, m_Ratio);

    return FilterInfo { dw, dh, info.bitCount, dst };
}
