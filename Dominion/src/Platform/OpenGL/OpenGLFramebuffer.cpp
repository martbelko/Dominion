#include "dmpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Dominion {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferDesc& desc)
		: m_Desc(desc)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		Delete();
	}

	void OpenGLFramebuffer::Invalidate()
	{
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Desc.Width, m_Desc.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentID, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthStencilAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_DepthStencilAttachmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Desc.Width, m_Desc.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilAttachmentID, 0);

		DM_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Desc.Width, m_Desc.Height);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		size_t size = static_cast<size_t>(std::max(m_Desc.Width, width)) * std::max(m_Desc.Height, height) * 4;
		int8_t* pixels = new int8_t[size];
		memset(pixels, 1, size);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		m_Desc.Width = width;
		m_Desc.Height = height;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Desc.Width, m_Desc.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glBindTexture(GL_TEXTURE_2D, m_DepthStencilAttachmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Desc.Width, m_Desc.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

		delete[] pixels;
	}

	uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID() const
	{
		return m_ColorAttachmentID;
	}

	const FramebufferDesc& OpenGLFramebuffer::GetDesc() const
	{
		return m_Desc;
	}

	void OpenGLFramebuffer::Delete()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachmentID);
		glDeleteTextures(1, &m_DepthStencilAttachmentID);
	}

}
