#include <chrono>
#include <cstdio>
#include <iostream>
#include "BitmapConverter.h"
#include "Bitmap.h"
#include "filters/BinaryFilter.h"
#include "filters/GaussianFilter.h"
#include "filters/GrayscaleFilter.h"
#include "filters/HsvFilter.h"
#include "filters/IImageFilter.h"
#include "filters/LaplacianFilter.h"
#include "filters/MosaicFilter.h"
#include "filters/NearestNeighborFilter.h"
#include "filters/NegativeFilter.h"

using namespace std;
using namespace ragii::image;

void BitmapConverter::applyFilter(Bitmap* bmp, FilterType type)
{
    FilterInfo info;
    info.width = bmp->getWidth();
    info.height = bmp->getHeight();
    info.bitCount = bmp->getBitCount();
    info.image = bmp->getData();

    unique_ptr<IImageFilter> filter;

    switch (type) {
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
        case FilterType::Hsv:
            filter = make_unique<HsvFilter>();
            break;
        case FilterType::NearestNeighbor:
            filter = make_unique<NearestNeighborFilter>(2.0f);
            break;
    }

    if (filter) {
        auto start = chrono::system_clock::now();

        info = filter->apply(info);
        bmp->setData(info.image);

        auto end = chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << "elapsed: " << elapsed << " us" << endl;
    }
}
