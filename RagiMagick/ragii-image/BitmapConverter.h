#pragma once

#include "filters/types.h"

namespace ragii { namespace image {

	class Bitmap;

	// TODO(YouGi): このクラスはあとで捨てる
	class BitmapConverter
	{
	public:
		static void applyFilter(Bitmap* bmp, FilterType type);
	};

}  // namespace image
 }  // namespace ragii
