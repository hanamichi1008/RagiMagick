#pragma once

#include "IImageFilter.h"

namespace ragii
{
    namespace image
    {
        class GaussianFilter : public IImageFilter
        {
        public:
            virtual ~GaussianFilter() override {}
            FilterInfo apply(const FilterInfo& info) override;
        };

    }  // namespace image
}  // namespace ragii
