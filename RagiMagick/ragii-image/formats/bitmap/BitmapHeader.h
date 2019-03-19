#pragma once

#include <cstdint>
#include "BitmapFileHeader.h"
#include "BitmapInfoHeader.h"

namespace ragii { namespace image {

#pragma pack(push, 1)

	struct BitmapHeader
	{
		BitmapFileHeader File;
		BitmapInfoHeader Info;
	};

#pragma pack(pop)

	constexpr size_t BitmapHeaderSize = sizeof(BitmapHeader);

}  // namespace image
 }  // namespace ragii
