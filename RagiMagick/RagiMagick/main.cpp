#include "pch.h"


using namespace std;


enum class BitmapVersion
{
	V1,
	V4,
	V5
};

struct BitmapHeder
{
	BITMAPFILEHEADER file;

	union BitmapInfoHeader
	{
		BITMAPINFOHEADER basic;
		BITMAPV4HEADER v4;
		BITMAPV5HEADER v5;
	} info;

	BitmapVersion version = BitmapVersion::V1;

	size_t getInfoSize() const
	{
		switch (version)
		{
		case BitmapVersion::V1:
			return sizeof(BITMAPINFOHEADER);
		case BitmapVersion::V4:
			return sizeof(BITMAPV4HEADER);
		case BitmapVersion::V5:
			return sizeof(BITMAPV5HEADER);
		}
	}
};



class Bitmap
{
public:
	static std::unique_ptr<Bitmap> loadFromFile(string path)
	{
		ifstream fs(path, ios::in | ios::binary);

		if (!fs)
		{
			return nullptr;
		}

		auto fileSize = fs.seekg(0, ios::end).tellg();

		if (sizeof(BITMAPFILEHEADER) > fileSize)
		{
			return nullptr;
		}

		auto bmp = std::make_unique<Bitmap>();

		fs.seekg(0, ios::beg);
		fs.read(reinterpret_cast<char*>(&bmp->m_Header.file), sizeof(BITMAPFILEHEADER));

		auto remain = fileSize - static_cast<streampos>(sizeof(BITMAPFILEHEADER));

		if (sizeof(BITMAPINFOHEADER) > remain)
		{
			return bmp;
		}

		DWORD infoSize = 0UL;
		fs.read(reinterpret_cast<char*>(&infoSize), sizeof(DWORD));
		fs.seekg(-streamoff(sizeof(DWORD)), ios::cur);

		if (infoSize == sizeof(BITMAPINFOHEADER))
		{
			fs.read(reinterpret_cast<char*>(&bmp->m_Header.info), sizeof(BITMAPINFOHEADER));
			bmp->m_Header.version = BitmapVersion::V1;
		}
		else if (infoSize == sizeof(BITMAPV4HEADER))
		{
			fs.read(reinterpret_cast<char*>(&bmp->m_Header.info), sizeof(BITMAPV4HEADER));
			bmp->m_Header.version = BitmapVersion::V4;
		}
		else if (infoSize == sizeof(BITMAPV5HEADER))
		{
			fs.read(reinterpret_cast<char*>(&bmp->m_Header.info), sizeof(BITMAPV5HEADER));
			bmp->m_Header.version = BitmapVersion::V5;
		}

		remain -= infoSize;

		fs.seekg(streamoff(bmp->m_Header.file.bfOffBits), ios::beg);
		if (fs.bad())
		{
			return bmp;
		}

		bmp->m_Data.reset((uint8_t*)malloc(bmp->m_Header.info.basic.biSizeImage));
		fs.read(reinterpret_cast<char*>(bmp->m_Data.get()), bmp->m_Header.info.basic.biSizeImage);

		fs.close();

		return bmp;
	}

	static std::unique_ptr<Bitmap> create(int32_t width, int32_t height, int32_t bitCount = 24)
	{
		auto bmp = std::make_unique<Bitmap>();

		bmp->m_Header.file = {};
		bmp->m_Header.file.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * bitCount / 8; // 適当
		bmp->m_Header.file.bfType = 'B' | ('M' << 8);
		bmp->m_Header.file.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		bmp->m_Header.info = {};
		bmp->m_Header.info.basic.biSize = sizeof(BITMAPINFOHEADER);
		bmp->m_Header.info.basic.biBitCount = 24;
		bmp->m_Header.info.basic.biWidth = width;
		bmp->m_Header.info.basic.biHeight = height;
		bmp->m_Header.info.basic.biPlanes = 1;
		bmp->m_Header.info.basic.biCompression = BI_RGB;
		bmp->m_Header.info.basic.biSizeImage = width * height * bitCount / 8;

		bmp->m_Data.reset((uint8_t*)malloc(bmp->m_Header.info.basic.biSizeImage));

		return bmp;
	}

	void save(string path)
	{
		ofstream fs(path, ios::out | ios::binary);
		fs.write(reinterpret_cast<char*>(&m_Header.file), sizeof(m_Header.file));
		fs.write(reinterpret_cast<char*>(&m_Header.info), m_Header.getInfoSize());
		fs.write(reinterpret_cast<char*>(m_Data.get()), m_Header.info.basic.biSizeImage);
		fs.flush();
		fs.close();
	}

	const BitmapHeder& getHeader() const
	{
		return m_Header;
	}

	std::unique_ptr<uint8_t[]>& getData()
	{
		return m_Data;
	}

	const std::unique_ptr<uint8_t[]>& getData() const
	{
		return m_Data;
	}

	int32_t getWidth() const
	{
		return m_Header.info.basic.biWidth;
	}

	int32_t getHeight() const
	{
		return m_Header.info.basic.biHeight;
	}

	int32_t getBitCount() const
	{
		return m_Header.info.basic.biBitCount;
	}

private:
	BitmapHeder m_Header = {};
	std::unique_ptr<uint8_t[]> m_Data;
};


class BitmapConverter
{
public:
	static void negative(const Bitmap* src, Bitmap* dst)
	{
		auto src_data = src->getData().get();
		auto dst_data = dst->getData().get();

		int w = src->getWidth();
		int h = src->getHeight();
		int d = src->getBitCount() / 8;

		for (int i = 0; i < w * h * d; i += d)
		{
			if (d != 3)
			{
				continue;
			}

			uint8_t r = 0xff - *(src_data + i + 0);
			uint8_t g = 0xff - *(src_data + i + 1);
			uint8_t b = 0xff - *(src_data + i + 2);

			*(dst_data + i + 0) = r;
			*(dst_data + i + 1) = g;
			*(dst_data + i + 2) = b;
		}
	}
};



int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		return EXIT_FAILURE;
	}

	vector<pair<string_view, string_view>> opts;

	for (size_t i = 1; i < argc; )
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

			auto nega = Bitmap::create(bmp->getWidth(), bmp->getHeight(), bmp->getBitCount());
			BitmapConverter::negative(bmp.get(), nega.get());
			nega->save(out_file->second.data());
		}
	}

	return EXIT_SUCCESS;
}
