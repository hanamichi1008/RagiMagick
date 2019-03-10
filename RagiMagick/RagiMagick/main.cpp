#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include "image/Bitmap.h"
#include "image/BitmapConverter.h"
#include "image/filters/GrayscaleFilter.h"

using namespace std;


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		return EXIT_FAILURE;
	}

	vector<pair<string_view, string_view>> opts;

	for (int i = 1; i < argc; )
	{
		auto opt = string_view(argv[i]);
		if (opt[0] == '-')
		{
			opts.emplace_back(argv[i], argv[i + 1]);
			i += 2;
		}
		else
		{
			opts.emplace_back(argv[i], "");
			i++;
		}
	}

	for (auto i : opts)
	{
		cout << i.first.data() << ": " << i.second.data() << endl;
	}

	if (!opts.empty() && opts[0].first == "convert")
	{
		opts.erase(opts.begin());
		if (!opts.empty() && opts[0].first == "negate")
		{
			opts.erase(opts.begin());

			auto in_file = find_if(opts.begin(), opts.end(), [&](auto i) { return i.first == "-i"; });
			if (in_file == opts.end())
			{
				cout << "not specified: -i" << endl;
				return EXIT_FAILURE;
			}

			auto out_file = find_if(opts.begin(), opts.end(), [&](auto i) { return i.first == "-o"; });
			if (out_file == opts.end())
			{
				cout << "not specified: -o" << endl;
				return EXIT_FAILURE;
			}

			cout << "input: " << in_file->second.data() << endl;
			cout << "output: " << out_file->second.data() << endl;

			auto bmp = Bitmap::loadFromFile(in_file->second.data());

			if (!bmp)
			{
				cout << "bitmap file load failed." << endl;
				return 0;
			}

			cout << (char)(bmp->getHeader().file.bfType & 0x00ff) << endl;
			cout << (char)(bmp->getHeader().file.bfType >> 8) << endl;

			//BitmapConverter::negative(bmp.get(), bmp.get());
			//bmp->save(out_file->second.data());

			//auto nega = Bitmap::create(bmp->getWidth(), bmp->getHeight(), bmp->getBitCount());
			//BitmapConverter::negative(bmp.get(), nega.get());
			//nega->save(out_file->second.data());

			ragii::image::FilterParams params;
			params.width = bmp->getWidth();
			params.height = bmp->getHeight();
			params.image = bmp->getData().get();
			ragii::image::GrayscaleFilter gs;
			gs.setFilterParams(params);
			gs.apply();
			bmp->save(out_file->second.data());
		}
	}

	return EXIT_SUCCESS;
}
