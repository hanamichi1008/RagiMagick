#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include "ragii/include/text/text.h"
#include "ragii/include/hardware/cpu_info.h"
#include "ragii-image/include/Bitmap.h"
#include "ragii-image/include/BitmapConverter.h"
#include "ragii-image/include/util.h"

using namespace std;
using namespace ragii::image;
using namespace ragii::text;
using namespace ragii::hardware;

void dumpSystemInfo();
int process(int argc, char* argv[]);
int convert();
int create();

map<string, string> g_opts;

inline bool has_opts()
{
    return !g_opts.empty();
}
inline bool contains_key(string_view key)
{
    return g_opts.count(key.data()) > 0;
}
inline bool has_value(string_view key)
{
    return !g_opts[key.data()].empty();
}
template<typename T>
inline T get_value(string_view key)
{
    return str_to_arithmetic<T>(g_opts[key.data()].c_str());
}
template<>
inline string get_value(string_view key)
{
    return g_opts[key.data()];
}

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

    string_view command;

    for (int i = 1; i < argc;) {
        auto opt = string_view(argv[i]);
        if (opt[0] == '-') {
            auto value = i + 1 < argc ? argv[i + 1] : "";
            g_opts[argv[i]] = value;
            i += 2;
        }
        else {
            command = string_view(argv[i]);
            i++;
        }
    }

    cout << command << endl;

    for (auto [k, v] : g_opts) {
        cout << k << ": " << v << endl;
    }

    if (command == "convert") {
        return convert();
    }
    if (command == "create") {
        return create();
    }

    cout << "コマンドを指定してください。 convert, etc." << endl;
    return EXIT_FAILURE;
}

int convert()
{
    if (!has_opts()) {
        cout << "オプションを指定してください" << endl;
        return EXIT_FAILURE;
    }

    if (!contains_key("-f") || !has_value("-f")) {
        cout << "-f 変換方法を指定してください。 negative, grayscale, etc." << endl;
        return EXIT_FAILURE;
    }

    if (!contains_key("-i") || !has_value("-i")) {
        cout << "入力ファイル名を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    if (!contains_key("-o") || !has_value("-o")) {
        cout << "出力ファイル名を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    unique_ptr<Bitmap> bmp;

    const auto& in_file = get_value<string>("-i").c_str();
    const auto& out_file = get_value<string>("-o").c_str();

    if (ends_with(in_file, ".bmp")) {
        cout << "start Bitmap::loadFromFile" << endl;
        bmp = Bitmap::loadFromFile(in_file);
    }
    else if (ends_with(in_file, ".jpg")) {
        cout << "start jpeg_to_bmp" << endl;
        bmp = jpeg_to_bmp(in_file);
    }
    else if (ends_with(in_file, ".png")) {
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

    const auto& filter = get_value<string>("-f");

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
        // BitmapConverter::applyFilter(bmp.get(), FilterType::Grayscale);
        BitmapConverter::applyFilter(bmp.get(), FilterType::Binary);
        BitmapConverter::applyFilter(bmp.get(), FilterType::Laplacian);
    }
    else if (filter == "gaussian") {
        BitmapConverter::applyFilter(bmp.get(), FilterType::Gaussian);
    }
    else if (filter == "mosaic") {
        BitmapConverter::applyFilter(bmp.get(), FilterType::Mosaic);
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
    if (has_opts()) {
        cout << "オプションを指定してください" << endl;
        return EXIT_FAILURE;
    }

    if (!contains_key("-o") || !has_value("-o")) {
        cout << "出力ファイル名を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    if (!contains_key("-w") || !has_value("-w")) {
        cout << "-w (幅) を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    if (!contains_key("-h") || !has_value("-h")) {
        cout << "-h (高さ) を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    if (!contains_key("-d") || !has_value("-d")) {
        cout << "-d (ビット深度 3: 24bit, 4: 32bit) を指定してください。" << endl;
        return EXIT_FAILURE;
    }
    if (!is_digit(get_value<string>("-d")[0])) {
        cout << "ビット深度の値が不正です。" << endl;
        return EXIT_FAILURE;
    }

    if (!contains_key("-p") || !has_value("-p")) {
        cout << "-p (パターン) を指定してください。" << endl;
        return EXIT_FAILURE;
    }

    auto bmp =
        Bitmap::create(
            get_value<int32_t>("-w"),
            get_value<int32_t>("-h"),
            get_value<int32_t>("-d") * 8);

    if (get_value<string>("-p") == "checkered") {
        auto data = bmp->getData();
        auto depth = bmp->getBitCount() / 8;
        for (int y = 0; y < bmp->getHeight(); y++) {
            for (int x = 0; x < bmp->getWidth(); x++) {
                for (int i = 0; i < depth; i++) {
                    *data++ = static_cast<uint8_t>(y ^ x);
                }
            }
        }
    }

    bmp->save(get_value<string>("-o").c_str());

    cout << "created." << endl;

    return EXIT_SUCCESS;
}
