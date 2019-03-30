#pragma once

#include "IImageFilter.h"

namespace ragii
{
    namespace image
    {
        class HsvFilter : public IImageFilter
        {
        public:
            virtual ~HsvFilter() override {}
            FilterInfo apply(const FilterInfo& info) override;
        };

    }  // namespace image
}  // namespace ragii
