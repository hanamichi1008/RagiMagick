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
            void apply() override;
        };

    }  // namespace image
}  // namespace ragii
