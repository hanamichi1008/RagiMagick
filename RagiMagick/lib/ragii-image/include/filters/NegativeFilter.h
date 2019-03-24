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
            void apply() override;
        };

    }  // namespace image
}  // namespace ragii
