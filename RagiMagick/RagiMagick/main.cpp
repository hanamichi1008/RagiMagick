#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include "ragii/image/Bitmap.h"
#include "ragii/image/BitmapConverter.h"
#include "ragii/text/text.h"


using namespace std;
using namespace ragii::image;
using namespace ragii::text;

struct CommandOption
{
	string_view name;
	string_view value;
};

int convert(vector<CommandOption>& opts);
int create(vector<CommandOption>& opts);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		return EXIT_FAILURE;
	}

	vector<CommandOption> opts;

	for (int i = 1; i < argc; )
	{
		auto opt = string_view(argv[i]);
		if (opt[0] == '-')
		{
			auto value = i + 1 < argc ? argv[i + 1] : "";
			opts.emplace_back(CommandOption { argv[i], value });
			i += 2;
		}
		else
		{
			opts.emplace_back(CommandOption { argv[i], "" });
			i++;
		}
	}

	for (auto i : opts)
	{
		cout << i.name.data() << ": " << i.value.data() << endl;
	}

	if (opts.empty())
	{
		cout << "コマンドを指定してください。 convert, etc." << endl;
		return EXIT_FAILURE;
	}

	auto command = opts[0].name;
	opts.erase(opts.begin());

	if (command == "convert")
	{
		return convert(opts);
	}
	else if (command == "create")
	{
		return create(opts);
	}

	return EXIT_SUCCESS;
}


int convert(vector<CommandOption>& opts)
{
	if (opts.empty())
	{
		cout << "変換方法を指定してください。 negative, grayscale, etc." << endl;
		return EXIT_FAILURE;
	}

	auto in_file = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-i"; });
	if (in_file == opts.end())
	{
		cout << "入力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (in_file->value.empty())
	{
		cout << "入力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	auto out_file = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-o"; });
	if (out_file == opts.end())
	{
		cout << "出力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (out_file->value.empty())
	{
		cout << "出力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	if (!ends_with(in_file->value.data(), ".bmp"))
	{
		cout << ".bmp 以外は非対応です。" << endl;
		return EXIT_FAILURE;
	}

	auto bmp = Bitmap::loadFromFile(in_file->value.data());

	if (!bmp)
	{
		cout << "ファイルのロードに失敗しました。" << endl;
		return EXIT_FAILURE;
	}

	cout << "width: " << bmp->getWidth() << ", height: " << bmp->getHeight() << ", depth: " << bmp->getBitCount() / 8 << endl;

	auto filter = opts[0].name;

	if (filter == "negative")
	{
		BitmapConverter::applyFilter(bmp.get(), FilterType::Negative);
	}
	else if (filter == "binary")
	{
		BitmapConverter::applyFilter(bmp.get(), FilterType::Binary);
	}
	else if (filter == "grayscale")
	{
		BitmapConverter::applyFilter(bmp.get(), FilterType::Grayscale);
	}
	else if (filter == "laplacian")
	{
		BitmapConverter::applyFilter(bmp.get(), FilterType::Grayscale);
		BitmapConverter::applyFilter(bmp.get(), FilterType::Laplacian);
	}

	bmp->save(out_file->value.data());
	cout << "converted." << endl;

	return EXIT_SUCCESS;
}

// RagiMagick create -w 32 -h 32 -d 3 -p checkered -o out.bmp
int create(vector<CommandOption>& opts)
{
	if (opts.empty())
	{
		cout << "オプションが不足しています。" << endl;
		return EXIT_FAILURE;
	}

	auto out_file = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-o"; });
	if (out_file == opts.end())
	{
		cout << "出力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (out_file->value.empty())
	{
		cout << "出力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	auto w = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-w"; });
	if (w == opts.end())
	{
		cout << "-w (幅) を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (w->value.empty())
	{
		cout << "幅の値を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	auto h = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-h"; });
	if (h == opts.end())
	{
		cout << "-h (高さ) を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (h->value.empty())
	{
		cout << "高さの値を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	auto d = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-d"; });
	if (d == opts.end())
	{
		cout << "-d (ビット深度 3: 24bit, 4: 32bit) を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (d->value.empty())
	{
		cout << "ビット深度の値を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (!is_digit(d->value.data()[0]))
	{
		cout << "ビット深度の値が不正です。" << endl;
		return EXIT_FAILURE;
	}

	auto p = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-p"; });
	if (p == opts.end())
	{
		cout << "-p (パターン) を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	auto bmp = 
		Bitmap::create(
			str_to_arithmetic<int32_t>(w->value.data()),
			str_to_arithmetic<int32_t>(h->value.data()),
			str_to_arithmetic<int16_t>(d->value.data()) * 8
		);

	if (p->value == "checkered")
	{
		auto data = bmp->getData().get();
		auto depth = bmp->getBitCount() / 8;
		for (int x = 0; x < bmp->getHeight(); x++)
		{
			for (int y = 0; y < bmp->getWidth(); y++)
			{
				for (int i = 0; i < depth; i++)
				{
					*data++ = y ^ x;
				}
			}
		}
	}

	bmp->save(out_file->value.data());

	cout << "created." << endl;

	return EXIT_SUCCESS;
}