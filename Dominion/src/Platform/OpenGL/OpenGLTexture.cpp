#include "dmpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace Dominion {

	OpenGLTexture2D::OpenGLTexture2D(const std::string_view& path)
	{
		DM_PROFILE_FUNCTION();

		int width, height, nChannels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data;
		{
			DM_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string_view& path)");
			data = stbi_load(path.data(), &width, &height, &nChannels, 0);
		}

		DM_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		if (nChannels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (nChannels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else
		{
			m_InternalFormat = 0;
			m_DataFormat = 0;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height), m_InternalFormat(GL_RGBA8), m_DataFormat(GL_RGBA)
	{
		DM_PROFILE_FUNCTION();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		DM_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	uint32_t OpenGLTexture2D::GetWidth() const
	{
		return m_Width;
	}

	uint32_t OpenGLTexture2D::GetHeight() const
	{
		return m_Height;
	}

	void OpenGLTexture2D::SetData(const void* data, uint32_t size)
	{
		DM_PROFILE_FUNCTION();

		DM_CORE_ASSERT(size == m_Width * m_Height * (m_DataFormat == GL_RGBA ? 4 : 3), "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind() const
	{
		DM_PROFILE_FUNCTION();

		glBindTextureUnit(0, m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		DM_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}

	uint32_t OpenGLTexture2D::GetRendererID() const
	{
		return m_RendererID;
	}

	bool OpenGLTexture2D::operator==(const Texture2D& other) const
	{
		return m_RendererID == reinterpret_cast<const OpenGLTexture2D&>(other).m_RendererID;
	}

}
