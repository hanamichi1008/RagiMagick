// libjpeg 使用前に stdio.h が必須
// https://github.com/libjpeg-turbo/libjpeg-turbo/issues/17
#include <stdio.h>
#include <jpeglib.h>
#include "Bitmap.h"
#include "util.h"

using namespace std;

namespace ragii::image
{
    unique_ptr<Bitmap> jpeg_to_bmp(string path)
    {
        FILE* fp = nullptr;

        auto err = fopen_s(&fp, path.c_str(), "rb");
        if (err != 0) {
            return nullptr;
        }

        unique_ptr<FILE, decltype(&fclose)> managed_fp(fp, fclose);

        jpeg_error_mgr err_mgr = {};
        jpeg_decompress_struct decompress_info = {};
        jpeg_create_decompress(&decompress_info);
        decompress_info.err = jpeg_std_error(&err_mgr);
        decompress_info.do_block_smoothing = FALSE;
        decompress_info.do_fancy_upsampling = FALSE;

        jpeg_stdio_src(&decompress_info, fp);
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

}  // namespace ragii::image
