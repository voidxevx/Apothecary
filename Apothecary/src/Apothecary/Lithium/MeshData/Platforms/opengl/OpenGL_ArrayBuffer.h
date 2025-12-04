#pragma once

#include "../../ArrayBuffer.h"

namespace apothec::lithium
{

	class OpenGLArrayBuffer : public ArrayBuffer
	{
	public:
		OpenGLArrayBuffer(float* data, size_t size);
		virtual ~OpenGLArrayBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		unsigned int m_RenderID;
	};

}