// libjpeg 使用前に stdio.h が必須
// https://github.com/libjpeg-turbo/libjpeg-turbo/issues/17
#include <stdio.h>
#include <jpeglib.h>
#include <png.h>
#include "Bitmap.h"
#include "util.h"

using namespace std;

namespace ragii::image
{
    unique_ptr<Bitmap> jpeg_to_bmp(string path)
    {
        unique_ptr<FILE, decltype(&fclose)> fp(fopen(path.c_str(), "rb"), fclose);
        if (!fp) {
            return nullptr;
        }

        jpeg_error_mgr err_mgr = {};
        jpeg_decompress_struct decompress_info = {};
        jpeg_create_decompress(&decompress_info);
        decompress_info.err = jpeg_std_error(&err_mgr);
        decompress_info.do_block_smoothing = FALSE;
        decompress_info.do_fancy_upsampling = FALSE;

        jpeg_stdio_src(&decompress_info, fp.get());
        jpeg_read_header(&decompress_info, TRUE);
        jpeg_start_decompress(&decompress_info);

        auto bmp =
            Bitmap::create(
                decompress_info.output_width,
                decompress_info.output_height,
                static_cast<int16_t>(decompress_info.output_components * 8));

        if (!bmp) {
            return nullptr;
        }

        auto img = bmp->getData();
        size_t lines = 0;
        size_t stride = decompress_info.output_width * decompress_info.output_components;

        while (decompress_info.output_scanline < decompress_info.output_height) {
            lines = jpeg_read_scanlines(&decompress_info, reinterpret_cast<JSAMPARRAY>(&img), 1);
            img += lines * stride;
        }

        jpeg_finish_decompress(&decompress_info);
        jpeg_destroy_decompress(&decompress_info);

        return bmp;
    }

    unique_ptr<Bitmap> png_to_bmp(string path)
    {
        int ret = 0;

        png_image image;
        memset(&image, 0, sizeof(png_image));
        image.version = PNG_IMAGE_VERSION;

        ret = png_image_begin_read_from_file(&image, path.c_str());
        if (ret == 0) {
            return nullptr;
        }
        if (PNG_IMAGE_FAILED(image)) {
            return nullptr;
        }

        size_t components = PNG_IMAGE_PIXEL_CHANNELS(image.format);

        auto bmp =
            Bitmap::create(
                image.width,
                image.height,
                static_cast<int16_t>(components * 8));

        if (!bmp) {
            return nullptr;
        }

        auto stride = PNG_IMAGE_ROW_STRIDE(image);

        ret = png_image_finish_read(&image, nullptr, bmp->getData(), stride, nullptr);
        if (ret == 0) {
            return nullptr;
        }

        png_image_free(&image);

        return bmp;
    }

}  // namespace ragii::image
