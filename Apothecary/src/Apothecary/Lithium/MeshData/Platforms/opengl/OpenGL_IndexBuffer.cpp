#include "OpenGL_IndexBuffer.h"

#include <glad/glad.h>

namespace apothec::lithium
{

	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* data, size_t size)
	{
		m_RenderID = 0;
		glGenBuffers(1, &m_RenderID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		Unbind();
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_RenderID);
	}

	void
	OpenGLIndexBuffer::Bind()
	const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
	}

	void
	OpenGLIndexBuffer::Unbind()
	const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}