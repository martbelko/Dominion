#include "dmpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Dominion {

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		DM_PROFILE_FUNCTION();

		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		DM_PROFILE_FUNCTION();

		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		DM_PROFILE_FUNCTION();

		glDeleteBuffers(1, &mRendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		DM_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		DM_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: mCount(count)
	{
		DM_PROFILE_FUNCTION();

		glCreateBuffers(1, &mRendererID);

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		DM_PROFILE_FUNCTION();

		glDeleteBuffers(1, &mRendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		DM_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		DM_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
