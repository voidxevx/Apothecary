#pragma once

#include "BismuthData.h"

#ifndef BISMUTH_STREAM_SIZE
	#define BISMUTH_STREAM_SIZE 16
#endif

namespace bismuth
{

	/*
	 * Holds a stack of input data and output data that can be accessed and pushed too.
	 * Input Stream holds values that can be accessed.
	 * Output Stream holds all of the values that should be stored.
	 * Values in the output stream are pushed into the input stream when streams are flipped.
	 */
	class DataIO
	{
	public:
		DataIO();
		~DataIO();

		/*
		 * Pushes data into the output stream
		 */
		void PushData(const DataPtr& data);
		/*
		 * Pops data from the input stream
		 */
		DataPtr PopData();

		/*
		 * Pushes data from output stream into input stream and wipes data in input stream.
		 */
		void FlipData();

	private:
		DataPtr* m_OutputStream;
		size_t m_OutputSize = 0;
		
		DataPtr* m_InputStream;
		size_t m_InputSize = 0;
	};

}