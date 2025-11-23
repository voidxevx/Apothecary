#include "DataIOStream.h"

namespace bismuth
{

	DataIO::DataIO()
	{
		m_DataStack = (DataPtr*)malloc(BISMUTH_STREAM_SIZE * sizeof(DataPtr));
	}

	DataIO::~DataIO()
	{
		free(m_DataStack);
		m_DataStack = nullptr;
	}

	void
	DataIO::PushData(DataPtr data)
	{
		m_DataStack[m_StackSize] = data;
		++m_StackSize;
	}

	DataPtr
	DataIO::PopData()
	{
		--m_StackSize;
		return m_DataStack[m_StackSize];
	}

}