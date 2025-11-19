#pragma once

#include "types.h"

#include <memory>

namespace bismuth
{

	struct DataPointer
	{
		void* const Data;
		PropertyID Type;

		DataPointer(void* const value, PropertyID type)
			: Data(value)
			, Type(type)
		{}
	};

	class IData
	{
	public:
		virtual DataPointer CastTo(DataPointer) const = 0;

	protected:
		IData()
		{}
	};

}