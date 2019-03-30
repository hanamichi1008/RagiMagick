#pragma once

#include "IImageFilter.h"

namespace ragii
{
    namespace image
    {
        class NegativeFilter : public IImageFilter
        {
        public:
            virtual ~NegativeFilter() override {}
            FilterInfo apply(const FilterInfo& info) override;
        };

    }  // namespace image
}  // namespace ragii
