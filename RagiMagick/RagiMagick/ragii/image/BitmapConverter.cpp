#include "BitmapConverter.h"
#include "Bitmap.h"
#include "filters/IImageFilter.h"
#include "filters/GrayscaleFilter.h"
#include "filters/NegativeFilter.h"
#include "filters/LaplacianFilter.h"


using namespace ragii::image;

using namespace std;


void BitmapConverter::applyFilter(Bitmap* bmp, FilterType type)
{
	FilterParams params;
	params.width = bmp->getWidth();
	params.height = bmp->getHeight();
	params.image = bmp->getData().get();

	unique_ptr<IImageFilter> filter;

	switch (type)
	{
	case FilterType::Grayscale:
		filter = make_unique<GrayscaleFilter>();
		break;
	case FilterType::Negative:
		filter = make_unique<NegativeFilter>();
		break;
	case FilterType::Laplacian:
		filter = make_unique<LaplacianFilter>();
		break;
	}

	if (filter)
	{
		filter->setFilterParams(params);
		filter->apply();
	}
}
