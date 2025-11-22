#include "DataIOStream.h"

namespace bismuth
{

	DataIO::DataIO()
	{
		m_OutputStream = (DataPtr*)malloc(BISMUTH_STREAM_SIZE * sizeof(DataPtr));
		m_InputStream = (DataPtr*)malloc(BISMUTH_STREAM_SIZE * sizeof(DataPtr));
	}

	DataIO::~DataIO()
	{
		free(m_OutputStream);
		free(m_InputStream);
		m_OutputStream = nullptr;
		m_InputStream = nullptr;
	}

	void
	DataIO::PushData(const DataPtr& data)
	{
		m_OutputStream[m_OutputSize] = data;
		++m_OutputSize;
	}

	DataPtr
	DataIO::PopData()
	{
		--m_InputSize;
		return m_InputStream[m_InputSize];
	}

	void
	DataIO::FlipData()
	{
		std::swap(m_InputStream, m_OutputStream);
		m_InputSize = m_OutputSize;
		m_OutputSize = 0;
	}

}