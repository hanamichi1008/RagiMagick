
#include <fstream>
#include "Bitmap.h"


using namespace std;


unique_ptr<Bitmap> Bitmap::loadFromFile(string path)
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

	auto bmp = make_unique<Bitmap>();

	fs.seekg(0, ios::beg);
	fs.read(reinterpret_cast<char*>(&bmp->m_Header.file), sizeof(BITMAPFILEHEADER));

	auto remain = fileSize - static_cast<streampos>(sizeof(BITMAPFILEHEADER));

	if (sizeof(BITMAPINFOHEADER) > remain)
	{
		return bmp;
	}

	uint64_t infoSize = 0UL;
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

	bmp->m_Data = make_unique<uint8_t[]>(bmp->m_Header.info.v1.biSizeImage);

	fs.read(reinterpret_cast<char*>(bmp->m_Data.get()), bmp->m_Header.info.v1.biSizeImage);

	fs.close();

	return bmp;
}

unique_ptr<Bitmap> Bitmap::create(int32_t width, int32_t height, int16_t bitCount)
{
	auto bmp = make_unique<Bitmap>();

	bmp->m_Header.file = {};
	bmp->m_Header.file.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * bitCount / 8;
	bmp->m_Header.file.bfType = 'B' | ('M' << 8);
	bmp->m_Header.file.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bmp->m_Header.info = {};
	bmp->m_Header.info.v1.biSize = sizeof(BITMAPINFOHEADER);
	bmp->m_Header.info.v1.biBitCount = bitCount;
	bmp->m_Header.info.v1.biWidth = width;
	bmp->m_Header.info.v1.biHeight = height;
	bmp->m_Header.info.v1.biPlanes = 1;
	bmp->m_Header.info.v1.biCompression = BI_RGB;
	bmp->m_Header.info.v1.biSizeImage = width * height * bitCount / 8;

	bmp->m_Data = make_unique<uint8_t[]>(bmp->m_Header.info.v1.biSizeImage);

	return bmp;
}

void Bitmap::save(string path)
{
	ofstream fs(path, ios::out | ios::binary);
	fs.write(reinterpret_cast<char*>(&m_Header.file), sizeof(m_Header.file));
	fs.write(reinterpret_cast<char*>(&m_Header.info), m_Header.getInfoSize());
	fs.write(reinterpret_cast<char*>(m_Data.get()), m_Header.info.v1.biSizeImage);
	fs.flush();
	fs.close();
}

const BitmapHeader& Bitmap::getHeader() const
{
	return m_Header;
}

unique_ptr<uint8_t[]>& Bitmap::getData()
{
	return m_Data;
}

const unique_ptr<uint8_t[]>& Bitmap::getData() const
{
	return m_Data;
}

int32_t Bitmap::getWidth() const
{
	return m_Header.info.v1.biWidth;
}

int32_t Bitmap::getHeight() const
{
	return m_Header.info.v1.biHeight;
}

int16_t Bitmap::getBitCount() const
{
	return m_Header.info.v1.biBitCount;
}

