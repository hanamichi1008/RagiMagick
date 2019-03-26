#pragma once

#include "IImageFilter.h"

namespace ragii
{
    namespace image
    {
        class GrayscaleFilter : public IImageFilter
        {
        public:
            virtual ~GrayscaleFilter() override {}
            void apply() override;
        };

    }  // namespace image
}  // namespace ragii
