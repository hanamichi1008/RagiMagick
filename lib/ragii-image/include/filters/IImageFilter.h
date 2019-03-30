﻿#pragma once

#include <cstdint>
#include <memory>

namespace ragii
{
    namespace image
    {
        struct FilterParams
        {
            int width;
            int height;
            int bitCount;
            //[[deprecated]] uint8_t* image;
            std::shared_ptr<uint8_t> image;
        };

        class IImageFilter
        {
        public:
            virtual ~IImageFilter() {}
            virtual void apply() = 0;

            void setFilterParams(const FilterParams& params) { m_Params = params; }

        protected:
            FilterParams m_Params;
        };

    }  // namespace image
}  // namespace ragii
