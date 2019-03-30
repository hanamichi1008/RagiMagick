#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <tuple>
#include "common.h"
#include "HsvFilter.h"

using namespace std;
using namespace ragii::image;

namespace
{
    constexpr double Epsilon = numeric_limits<float>::epsilon();

    inline auto rgb_to_hsv(float r, float g, float b)
    {
        float max = std::max({ b, g, r });
        float min = std::min({ b, g, r });
        float h = 0.0f;
        float v = max;
        float s = (max - min) / max;

        if (fabsf(max - min) <= Epsilon) {
            h = 0.0f;
        }
        else if (fabsf(r - max) <= Epsilon) {
            h = 60 * ((g - b) / (max - min));
        }
        else if (fabsf(g - max) <= Epsilon) {
            h = 60 * ((b - r) / (max - min)) + 120.0f;
        }
        else if (fabsf(b - max) <= Epsilon) {
            h = 60 * ((r - g) / (max - min)) + 240.0f;
        }

        if (h < 0.0f) {
            h += 360.0f;
        }

        return make_tuple(h, s, v);
    }

    inline auto hsv_to_rgb(float h, float s, float v)
    {
        float max = v;
        float min = max - (s * v);
        float r, g, b;

        switch ((int)floorf(h / 60.0f)) {
            case 0:
                r = max;
                g = (h / 60.0f) * (max - min) + min;
                b = min;
                break;
            case 1:
                r = ((120.0f - h) / 60.0f) * (max - min) + min;
                g = max;
                b = min;
                break;
            case 2:
                r = min;
                g = max;
                b = ((h - 120.0f) / 60.0f) * (max - min) + min;
                break;
            case 3:
                r = min;
                g = ((240.0f - h) / 60.0f) * (max - min) + min;
                b = max;
                break;
            case 4:
                r = ((h - 240.0f) / 60.0f) * (max - min) + min;
                g = min;
                b = max;
                break;
            case 5:
                r = max;
                g = min;
                b = ((360.0f - h) / 60.0f) * (max - min) + min;
                break;
        }

        return make_tuple(r, g, b);
    }

    inline void hsv_default(uint8_t* img, int width, int height, int depth)
    {
        float r, g, b;
        float h, s, v;

        for (int i = 0; i < width * height * depth; i += depth) {
            b = img[0] / 255.0f;
            g = img[1] / 255.0f;
            r = img[2] / 255.0f;

            tie(h, s, v) = rgb_to_hsv(r, g, b);

            // h 反転
            h = fmodf(h + 180.0f, 360.0f);

            tie(r, g, b) = hsv_to_rgb(h, s, v);

            img[0] = floor(b * 255.0f);
            img[1] = floor(g * 255.0f);
            img[2] = floor(r * 255.0f);
            img += depth;
        }
    }
}  // namespace

FilterInfo HsvFilter::apply(const FilterInfo& info)
{
    hsv_default(info.image.get(), info.width, info.height, info.bitCount / 8);

    return FilterInfo { info.width, info.height, info.bitCount, info.image };
}
