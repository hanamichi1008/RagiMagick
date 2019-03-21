#pragma once

#include "IImageFilter.h"


namespace ragii { namespace image {

	class MosaicFilter : public IImageFilter
	{
	public:
		void apply() override;
	};

} }
