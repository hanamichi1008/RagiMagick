#pragma once

#include <cstdint>

namespace ragii
{
namespace image
{
struct FilterParams {
    int width;
    int height;
    int bitCount;
    uint8_t* image;
};

class IImageFilter
{
public:
    virtual ~IImageFilter() = default;
    virtual void apply() = 0;

    void setFilterParams(const FilterParams& params) { m_Params = params; }

protected:
    FilterParams m_Params;
};

}  // namespace image
}  // namespace ragii
