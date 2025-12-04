#pragma once

#include "../../IndexBuffer.h"

namespace apothec::lithium
{

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int* data, size_t size);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		unsigned int m_RenderID;
	};

}