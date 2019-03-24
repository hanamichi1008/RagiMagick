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
            void apply() override;
        };

    }  // namespace image
}  // namespace ragii
