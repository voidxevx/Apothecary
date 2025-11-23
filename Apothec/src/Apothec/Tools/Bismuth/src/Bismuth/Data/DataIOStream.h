#pragma once

#include "BismuthData.h"

#ifndef BISMUTH_STREAM_SIZE
	#define BISMUTH_STREAM_SIZE 32
#endif

namespace bismuth
{

	/*
	 * Holds a stack of data that can be accessed and pushed too.
	 */
	class DataIO
	{
	public:
		DataIO();
		~DataIO();

		void PushData(DataPtr data);
		DataPtr PopData();

	private:
		DataPtr* m_DataStack;
		size_t m_StackSize = 0;
	};

}