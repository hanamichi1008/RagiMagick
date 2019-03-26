#pragma once

#include <string>
#include <memory>

namespace ragii::image
{
    class Bitmap;

    std::unique_ptr<Bitmap> jpeg_to_bmp(std::string path);
    std::unique_ptr<Bitmap> png_to_bmp(std::string path);

}  // namespace ragii::image
