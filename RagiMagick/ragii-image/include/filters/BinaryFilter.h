#pragma once

#include "IImageFilter.h"


namespace ragii { namespace image {

	class BinaryFilter : public IImageFilter
	{
	public:
		void apply() override;
	};

} }
