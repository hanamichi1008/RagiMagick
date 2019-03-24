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
            void apply() override;
        };

    }  // namespace image
}  // namespace ragii
