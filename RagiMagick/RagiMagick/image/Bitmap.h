#pragma once

#include <memory>
#include <string>
#include "types.h"

class Bitmap
{
public:
	static std::unique_ptr<Bitmap> loadFromFile(std::string path);
	static std::unique_ptr<Bitmap> create(int32_t width, int32_t height, int16_t bitCount = 24);

	void save(std::string path);

	const BitmapHeader& getHeader() const;

	std::unique_ptr<uint8_t[]>& getData();
	const std::unique_ptr<uint8_t[]>& getData() const;

	int32_t getWidth() const;
	int32_t getHeight() const;
	int16_t getBitCount() const;

private:
	BitmapHeader m_Header = {};
	std::unique_ptr<uint8_t[]> m_Data;
};

