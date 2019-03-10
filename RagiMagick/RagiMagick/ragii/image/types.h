#pragma once

#include <windows.h>

enum class BitmapVersion
{
	V1,
	V4,
	V5
};

struct BitmapHeader
{
	BITMAPFILEHEADER file;

	union BitmapInfoHeader
	{
		BITMAPINFOHEADER v1;
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

		return 0;
	}
};
