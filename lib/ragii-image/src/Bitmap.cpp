#include <fstream>
#include <iostream>
#include <string.h>
#include "Bitmap.h"
#include "formats/bitmap/BitmapFileHeader.h"
#include "formats/bitmap/BitmapInfoHeader.h"
#include "hardware/cpu_info.h"

using namespace ragii;
using namespace ragii::hardware;
using namespace ragii::image;
using namespace std;

namespace
{
    inline auto bgr24_to_bgra32_default(uint8_t* src, int width, int height)
    {
        auto dst = aligned_allocator<uint8_t>::make_unique(width * height * 4, 16);
        auto p = dst.get();

#if defined(__clang__)
#    pragma clang loop vectorize(disable)
#elif defined(_MSC_VER)
#    pragma loop(no_vector)
#endif
        for (int i = 0; i < width * height * 3; i += 3) {
            p[0] = src[i + 0];
            p[1] = src[i + 1];
            p[2] = src[i + 2];
            p[3] = 0;
            p += 4;
        }

        return dst;
    }

    inline auto bgr24_to_bgra32(uint8_t* src, int width, int height)
    {
        CpuInfo info;
        auto reg = info.load(1);
        CpuAvailableFeatures features(reg);

        features = CpuAvailableFeatures({}); // debug

        unique_ptr<uint8_t, aligned_deleter<uint8_t>> dst;

        if (features.avx2()) {
        }
        else if (features.avx()) {
        }
        else if (features.sse42()) {
        }
        else {
            dst = bgr24_to_bgra32_default(src, width, height);
        }

        return dst;
    }
}  // namespace

unique_ptr<Bitmap> Bitmap::loadFromFile(string path)
{
    ifstream fs(path, ios::in | ios::binary);

    if (!fs) {
        cout << path << ": open failed." << endl;
        return nullptr;
    }

    auto fileSize = fs.seekg(0, ios::end).tellg();

    if (BitmapFileHeaderSize > static_cast<size_t>(fileSize)) {
        cout << "invalid bitmap file header." << endl;
        return nullptr;
    }

    auto bmp = make_unique<Bitmap>();

    fs.seekg(0, ios::beg);
    fs.read(reinterpret_cast<char*>(&bmp->m_Header.File), BitmapFileHeaderSize);

    if (!((bmp->m_Header.File.Type & 0xff) == 'B') && (bmp->m_Header.File.Type >> 8) == 'M') {
        cout << "invalid bitmap file header." << endl;
        return nullptr;
    }

    auto remain = fileSize - static_cast<streampos>(BitmapFileHeaderSize);

    if (BitmapInfoHeaderSize > static_cast<size_t>(remain)) {
        cout << "invalid bitmap info header." << endl;
        return bmp;
    }

    uint32_t infoSize = 0UL;
    fs.read(reinterpret_cast<char*>(&infoSize), sizeof(uint32_t));
    fs.seekg(-streamoff(sizeof(uint32_t)), ios::cur);

    if (infoSize == BitmapInfoHeaderSize) {
        fs.read(reinterpret_cast<char*>(&bmp->m_Header.Info), BitmapInfoHeaderSize);
    }

    remain -= infoSize;

    fs.seekg(streamoff(bmp->m_Header.File.OffBits), ios::beg);
    if (fs.bad()) {
        return bmp;
    }

    uint32_t dataSize = static_cast<uint32_t>(fileSize - static_cast<streampos>(bmp->m_Header.File.OffBits));

    auto src = aligned_allocator<uint8_t>::make_unique(bmp->m_Header.Info.SizeImage, 16);
    fs.read(reinterpret_cast<char*>(src.get()), dataSize);

    // TODO: 16, 8
    if (bmp->getBitCount() == 24)
    {
        auto& file = bmp->m_Header.File;
        auto& info = bmp->m_Header.Info;

        auto dst = bgr24_to_bgra32(src.get(), info.Width, info.Height);
        bmp->m_Data = move(dst);

        // ヘッダ更新
        file.Size = BitmapHeaderSize + static_cast<uint32_t>(info.Width * info.Height * 32 / 8);
        info.BitCount = 32;
        info.SizeImage = static_cast<uint32_t>(info.Width * info.Height * 32 / 8);
    }
    else
    {
        bmp->m_Data = move(src);
    }

    fs.close();

    return bmp;
}

unique_ptr<Bitmap> Bitmap::create(int32_t width, int32_t height, int16_t bitCount)
{
    auto bmp = make_unique<Bitmap>();

    BitmapFileHeader file = {};
    file = {};
    file.Size = BitmapHeaderSize + static_cast<uint32_t>(width * height * bitCount / 8);
    file.Type = 'B' | ('M' << 8);
    file.OffBits = BitmapHeaderSize;
    memcpy(&bmp->m_Header.File, &file, BitmapFileHeaderSize);

    BitmapInfoHeader info = {};
    info.Size = BitmapInfoHeaderSize;
    info.BitCount = static_cast<uint16_t>(bitCount);
    info.Width = width;
    info.Height = height;
    info.Planes = 1;
    info.Compression = BI_RGB;
    info.SizeImage = static_cast<uint32_t>(width * height * bitCount / 8);
    memcpy(&bmp->m_Header.Info, &info, BitmapInfoHeaderSize);

    bmp->m_Data = aligned_allocator<uint8_t>::make_shared(bmp->m_Header.Info.SizeImage, 16);

    return bmp;
}

void Bitmap::save(string path)
{
    if (!m_Data) {
        return;
    }

    auto& info = m_Header.Info;

    ofstream fs(path, ios::out | ios::binary);
    fs.write(reinterpret_cast<char*>(&m_Header.File), BitmapFileHeaderSize);
    fs.write(reinterpret_cast<char*>(&m_Header.Info), BitmapInfoHeaderSize);
    fs.write(reinterpret_cast<char*>(m_Data.get()), info.Width * info.Height * info.BitCount / 8);
    fs.flush();
    fs.close();
}

const BitmapHeader& Bitmap::getHeader() const { return m_Header; }

int32_t Bitmap::getWidth() const { return m_Header.Info.Width; }

int32_t Bitmap::getHeight() const { return m_Header.Info.Height; }

uint16_t Bitmap::getBitCount() const { return m_Header.Info.BitCount; }
