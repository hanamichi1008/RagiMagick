#include <chrono>
#include <cstdio>
#include "Bitmap.h"
#include "BitmapConverter.h"
#include "filters/IImageFilter.h"
#include "filters/BinaryFilter.h"
#include "filters/GrayscaleFilter.h"
#include "filters/NegativeFilter.h"
#include "filters/LaplacianFilter.h"
#include "filters/GaussianFilter.h"
#include "filters/MosaicFilter.h"

using namespace std;
using namespace ragii::image;

void BitmapConverter::applyFilter(Bitmap* bmp, FilterType type)
{
	FilterParams params;
	params.width = bmp->getWidth();
	params.height = bmp->getHeight();
	params.bitCount = bmp->getBitCount();
	params.image = bmp->getData().get();

	unique_ptr<IImageFilter> filter;

	switch (type)
	{
	case FilterType::Binary:
		filter = make_unique<BinaryFilter>();
		break;
	case FilterType::Grayscale:
		filter = make_unique<GrayscaleFilter>();
		break;
	case FilterType::Negative:
		filter = make_unique<NegativeFilter>();
		break;
	case FilterType::Laplacian:
		filter = make_unique<LaplacianFilter>();
		break;
	case FilterType::Gaussian:
		filter = make_unique<GaussianFilter>();
		break;
	case FilterType::Mosaic:
		filter = make_unique<MosaicFilter>();
		break;
	}

	if (filter)
	{
		filter->setFilterParams(params);

		auto start = chrono::system_clock::now();

		filter->apply();

		auto end = chrono::system_clock::now();
		double elapsed = chrono::duration_cast<chrono::microseconds>(end - start).count();
		printf("elapsed: %.3lf us\n", elapsed);
	}
}
