#pragma once

#include "filters/types.h"

namespace ragii { namespace image {

	class Bitmap;

	// TODO: このクラスはあとで捨てる
	class BitmapConverter
	{
	public:
		static void applyFilter(Bitmap* bmp, FilterType type);
	};

} }
