#pragma once

#include "IImageFilter.h"

namespace ragii
{
    namespace image
    {
        class LaplacianFilter : public IImageFilter
        {
        public:
            virtual ~LaplacianFilter() override {}
            FilterInfo apply(const FilterInfo& info) override;
        };

    }  // namespace image
}  // namespace ragii
