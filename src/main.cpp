#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include "ragii/include/text/text.h"
#include "ragii/include/hardware/cpu_info.h"
#include "ragii-image/include/Bitmap.h"
#include "ragii-image/include/BitmapConverter.h"
#include "ragii-image/include/util.h"
#include "CommandLine.h"

using namespace std;
using namespace ragii::image;
using namespace ragii::text;
using namespace ragii::hardware;
using namespace RagiMagick;

void dumpSystemInfo();
int process(int argc, char* argv[]);
int convert();
int create();

unique_ptr<CommandLine> g_cmd;

int main(int argc, char* argv[])
{
    dumpSystemInfo();

    return process(argc, argv);
}

void dumpSystemInfo()
{
    CpuInfo info;
    cout << CpuVendor(info.load(0)).getName() << endl;

    auto reg = info.load(1);
    CpuAvailableFeatures features(reg);
    cout << "sse:   " << features.sse() << endl;
    cout << "sse2:  " << features.sse2() << endl;
    cout << "sse3:  " << features.sse3() << endl;
    cout << "sse41: " << features.sse41() << endl;
    cout << "sse42: " << features.sse42() << endl;
    cout << "avx:   " << features.avx() << endl;
    cout << "avx2:  " << features.avx2() << endl;
}

int process(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "コマンドを指定してください。 convert, etc." << endl;
        return EXIT_FAILURE;
    }

    g_cmd = CommandLine::parse(argc, argv);

    if (!g_cmd) {
        return EXIT_FAILURE;
    }

    if (g_cmd->getCommand() == "convert") {
        return convert();
    }
    if (g_cmd->getCommand() == "create") {
        return create();
    }

    cout << "コマンドを指定してください。 convert, etc." << endl;
    return EXIT_FAILURE;
}

int convert()
{
    if (!g_cmd->hasOptions()) {
        cout << "オプションを指定してください" << endl;
        return EXIT_FAILURE;
    }

    if (!g_cmd->containsKey("-f") || !g_cmd->hasValue("-f")) {
        cout << "-f 変換方法を指定してください。 negative, grayscale, etc." << endl;
        return EXIT_FAILURE;
    }

    if (!g_cmd->containsKey("-i") || !g_cmd->hasValue("-i")) {
        cout << "入力ファイル名を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    if (!g_cmd->containsKey("-o") || !g_cmd->hasValue("-o")) {
        cout << "出力ファイル名を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    unique_ptr<Bitmap> bmp;

    auto in_file = g_cmd->getValue<string>("-i");
    auto out_file = g_cmd->getValue<string>("-o");

    if (ends_with(in_file.c_str(), ".bmp")) {
        cout << "start Bitmap::loadFromFile" << endl;
        bmp = Bitmap::loadFromFile(in_file);
    }
    else if (ends_with(in_file.c_str(), ".jpg")) {
        cout << "start jpeg_to_bmp" << endl;
        bmp = jpeg_to_bmp(in_file);
    }
    else if (ends_with(in_file.c_str(), ".png")) {
        cout << "start png_to_bmp" << endl;
        bmp = png_to_bmp(in_file);
    }
    else {
        cout << "入力ファイルは .bmp, .jpg, .png 以外非対応です。" << endl;
        return EXIT_FAILURE;
    }

    if (!bmp) {
        cout << "ファイルのロードに失敗しました。" << endl;
        return EXIT_FAILURE;
    }

    cout << "width: " << bmp->getWidth() << ", height: " << bmp->getHeight() << ", depth: " << bmp->getBitCount() / 8 << endl;

    const auto& filter = g_cmd->getValue<string>("-f");

    if (filter == "negative") {
        BitmapConverter::applyFilter(bmp.get(), FilterType::Negative);
    }
    else if (filter == "binary") {
        BitmapConverter::applyFilter(bmp.get(), FilterType::Binary);
    }
    else if (filter == "grayscale") {
        BitmapConverter::applyFilter(bmp.get(), FilterType::Grayscale);
    }
    else if (filter == "laplacian") {
        BitmapConverter::applyFilter(bmp.get(), FilterType::Laplacian);
    }
    else if (filter == "gaussian") {
        BitmapConverter::applyFilter(bmp.get(), FilterType::Gaussian);
    }
    else if (filter == "mosaic") {
        BitmapConverter::applyFilter(bmp.get(), FilterType::Mosaic);
    }
    else if (filter == "hsv") {
        BitmapConverter::applyFilter(bmp.get(), FilterType::Hsv);
    }
    else if (filter == "nn") {
        bmp->scale(2.5);
    }
    else {
        cout << "未知のコマンドが指定されています！" << endl;
        return EXIT_FAILURE;
    }

    bmp->save(out_file);
    cout << "converted." << endl;

    return EXIT_SUCCESS;
}

// RagiMagick create -w 32 -h 32 -d 3 -p checkered -o out.bmp
int create()
{
    if (!g_cmd->hasOptions()) {
        cout << "オプションを指定してください" << endl;
        return EXIT_FAILURE;
    }

    if (!g_cmd->containsKey("-o") || !g_cmd->hasValue("-o")) {
        cout << "出力ファイル名を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    if (!g_cmd->containsKey("-w") || !g_cmd->hasValue("-w")) {
        cout << "-w (幅) を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    if (!g_cmd->containsKey("-h") || !g_cmd->hasValue("-h")) {
        cout << "-h (高さ) を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    if (!g_cmd->containsKey("-d") || !g_cmd->hasValue("-d")) {
        cout << "-d (ビット深度 3: 24bit, 4: 32bit) を指定してください。" << endl;
        return EXIT_FAILURE;
    }
    if (!is_digit(g_cmd->getValue<string>("-d")[0])) {
        cout << "ビット深度の値が不正です。" << endl;
        return EXIT_FAILURE;
    }

    if (!g_cmd->containsKey("-p") || !g_cmd->hasValue("-p")) {
        cout << "-p (パターン) を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    auto bmp =
        Bitmap::create(
            g_cmd->getValue<int32_t>("-w"),
            g_cmd->getValue<int32_t>("-h"),
            g_cmd->getValue<int32_t>("-d") * 8);

    if (g_cmd->getValue<string>("-p") == "checkered") {
        auto data = bmp->getData().get();
        auto depth = bmp->getBitCount() / 8;
        for (int y = 0; y < bmp->getHeight(); y++) {
            for (int x = 0; x < bmp->getWidth(); x++) {
                for (int i = 0; i < depth; i++) {
                    *data++ = static_cast<uint8_t>(y ^ x);
                }
            }
        }
    }

    bmp->save(g_cmd->getValue<string>("-o"));

    cout << "created." << endl;

    return EXIT_SUCCESS;
}
