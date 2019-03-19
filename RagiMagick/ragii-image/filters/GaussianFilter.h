#pragma once

#include "IImageFilter.h"


namespace ragii { namespace image {

	class GaussianFilter : public IImageFilter
	{
	public:
		void apply() override;
	};

} }
