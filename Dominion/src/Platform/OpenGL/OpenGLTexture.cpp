#include "dmpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace Dominion {

	OpenGLTexture2D::OpenGLTexture2D(const std::string_view& path)
	{

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{

	}

	uint32_t OpenGLTexture2D::GetWidth() const
	{
		return m_Width;
	}

	uint32_t OpenGLTexture2D::GetHeight() const
	{
		return m_Height;
	}

	void OpenGLTexture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

}
