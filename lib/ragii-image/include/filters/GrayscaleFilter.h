#pragma once

#include "IImageFilter.h"

namespace ragii
{
    namespace image
    {
        class GrayscaleFilter : public IImageFilter
        {
        public:
            virtual ~GrayscaleFilter() override {}
            FilterInfo apply(const FilterInfo& info) override;
        };

    }  // namespace image
}  // namespace ragii
