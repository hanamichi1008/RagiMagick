#pragma once

#include "IImageFilter.h"

namespace ragii
{
    namespace image
    {
        class MosaicFilter : public IImageFilter
        {
        public:
            virtual ~MosaicFilter() override {}
            FilterInfo apply(const FilterInfo& info) override;
        };

    }  // namespace image
}  // namespace ragii
