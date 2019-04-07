// libjpeg 使用前に stdio.h が必須
// https://github.com/libjpeg-turbo/libjpeg-turbo/issues/17
#include <stdio.h>
#include <string.h>
#include <jpeglib.h>
#include <png.h>
#include "Bitmap.h"
#include "util.h"
#include "common.h"

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
        jpeg_decompress_struct cinfo = {};
        jpeg_create_decompress(&cinfo);
        cinfo.err = jpeg_std_error(&err_mgr);
        cinfo.do_block_smoothing = FALSE;
        cinfo.do_fancy_upsampling = FALSE;

        jpeg_stdio_src(&cinfo, fp.get());
        jpeg_read_header(&cinfo, TRUE);
        jpeg_start_decompress(&cinfo);

        size_t alloc_size = cinfo.output_width * cinfo.output_height * static_cast<size_t>(cinfo.output_components);
        unique_aligned_ptr img = allocate(alloc_size, 16);

        size_t lines = 0;
        size_t stride = cinfo.output_width * static_cast<size_t>(cinfo.output_components);
        auto img_ptr = img.get();

        while (cinfo.output_scanline < cinfo.output_height) {
            lines = jpeg_read_scanlines(&cinfo, reinterpret_cast<JSAMPARRAY>(&img), 1);
            img_ptr += lines * stride;
        }

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);

        auto new_img = bgr24_to_bgra32(img.get(), cinfo.output_width, cinfo.output_height);
        auto bmp = Bitmap::loadFromRawData(cinfo.output_width, cinfo.output_height, 32, move(new_img));
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

        int width = image.width;
        int height = image.height;
        size_t components = PNG_IMAGE_PIXEL_CHANNELS(image.format);
        size_t alloc_size = width * height * static_cast<size_t>(components);
        unique_aligned_ptr img = allocate(alloc_size, 16);

        int32_t stride = -static_cast<int32_t>(PNG_IMAGE_ROW_STRIDE(image));

        ret = png_image_finish_read(&image, nullptr, img.get(), stride, nullptr);
        if (ret == 0) {
            return nullptr;
        }

        png_image_free(&image);

        if (components == 3) {
            auto new_img = bgr24_to_bgra32(img.get(), width, height);
            auto bmp = Bitmap::loadFromRawData(width, height, 32, move(new_img));
            return bmp;
        }

        auto bmp = Bitmap::loadFromRawData(width, height, components * 8, move(img));
        return bmp;
    }

}  // namespace ragii::image
