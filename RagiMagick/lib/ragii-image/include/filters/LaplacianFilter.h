#pragma once

#include "IImageFilter.h"

namespace ragii
{
    namespace image
    {
        class LaplacianFilter : public IImageFilter
        {
        public:
            void apply() override;
        };

    }  // namespace image
}  // namespace ragii
