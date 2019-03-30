#pragma once

#include "IImageFilter.h"

namespace ragii
{
    namespace image
    {
        class BinaryFilter : public IImageFilter
        {
        public:
            virtual ~BinaryFilter() override {}
            FilterInfo apply(const FilterInfo& info) override;
        };

    }  // namespace image
}  // namespace ragii
