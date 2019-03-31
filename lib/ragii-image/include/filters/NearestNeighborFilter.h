#pragma once

#include "IImageFilter.h"

namespace ragii
{
    namespace image
    {
        class NearestNeighborFilter : public IImageFilter
        {
        public:
            NearestNeighborFilter(float ratio);
            virtual ~NearestNeighborFilter() override {}
            FilterInfo apply(const FilterInfo& info) override;

        private:
            float m_Ratio = 1.0f;
        };

    }  // namespace image
}  // namespace ragii
