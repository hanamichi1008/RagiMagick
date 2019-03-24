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
            void apply() override;
        };

    }  // namespace image
}  // namespace ragii
