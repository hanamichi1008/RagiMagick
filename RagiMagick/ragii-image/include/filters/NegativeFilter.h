#pragma once

#include "IImageFilter.h"


namespace ragii { namespace image {

	class NegativeFilter : public IImageFilter
	{
	public:
		void apply() override;
	};

} }
