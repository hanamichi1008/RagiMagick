#pragma once

#include <cstdint>
#include <memory>

namespace ragii
{
    namespace image
    {
        struct FilterInfo
        {
            int width;
            int height;
            int bitCount;
            std::shared_ptr<uint8_t> image;
        };

        class IImageFilter
        {
        public:
            virtual ~IImageFilter() {}
            virtual FilterInfo apply(const FilterInfo& info) = 0;
        };

    }  // namespace image
}  // namespace ragii
