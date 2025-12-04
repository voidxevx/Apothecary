#include "OpenGL_ArrayBuffer.h"

#include <glad/glad.h>

namespace apothec::lithium
{
	ArrayBuffer*
	ArrayBuffer::CreateArrayBuffer(float* data, size_t size)
	{
		return new OpenGLArrayBuffer(data, size);
	}

	OpenGLArrayBuffer::OpenGLArrayBuffer(float* data, size_t size)
	{
		m_RenderID = 0;
		glGenBuffers(1, &m_RenderID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
		Unbind();
	}

	OpenGLArrayBuffer::~OpenGLArrayBuffer()
	{
		glDeleteBuffers(1, &m_RenderID);
	}

	void
	OpenGLArrayBuffer::Bind() 
	const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	}

	void
	OpenGLArrayBuffer::Unbind()
	const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


}