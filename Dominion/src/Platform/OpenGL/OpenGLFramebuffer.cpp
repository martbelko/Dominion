#include "dmpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Dominion {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferDesc& desc)
		: m_Desc(desc)
	{
		for (auto spec : desc.attachments.attachments)
		{
			if (!IsDepthFormat(spec.textureFormat))
				mColorAttachmentSpecs.emplace_back(spec);
			else
				mDepthAttachmentSpec = spec;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		Delete();
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			Delete();

			mColorAttachments.clear();
			mDepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Desc.samples > 1;

		// Attachments
		if (!mColorAttachmentSpecs.empty())
		{
			mColorAttachments.resize(mColorAttachmentSpecs.size());
			CreateTextures(multisample, mColorAttachments.data(), mColorAttachments.size());

			for (size_t i = 0; i < mColorAttachments.size(); ++i)
			{
				BindTexture(multisample, mColorAttachments[i]);
				switch (mColorAttachmentSpecs[i].textureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					AttachColorTexture(mColorAttachments[i], m_Desc.samples, GL_RGBA8, m_Desc.width, m_Desc.height, i);
					break;
				}
			}
		}

		if (mDepthAttachmentSpec.textureFormat != FramebufferTextureFormat::None)
		{
			CreateTextures(multisample, &mDepthAttachment, 1);
			BindTexture(multisample, mDepthAttachment);
			switch (mDepthAttachmentSpec.textureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(mDepthAttachment, m_Desc.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Desc.width, m_Desc.height);
				break;
			}
		}

		if (mColorAttachments.size() > 1)
		{
			DM_CORE_ASSERT(mColorAttachments.size() <= 4, "Engine currently support maximum of 4 color framebuffer color attachments");

			U32 buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(mColorAttachments.size(), buffers);
		}
		else if (mColorAttachments.empty())
		{
			// Depth-pass only
			glDrawBuffer(GL_NONE);
		}

		DM_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind in the end
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Desc.width, m_Desc.height);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(U32 width, U32 height)
	{
	#ifdef DM_DEBUG
		if (width == 0 || height == 0)
		{
			DM_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}
	#endif

		m_Desc.width = width;
		m_Desc.height = height;

		Invalidate();
	}

	U32 OpenGLFramebuffer::GetColorAttachmentRendererID(U32 index) const
	{
		DM_CORE_ASSERT(index >= 0 && index < mColorAttachments.size(), "Invalid color attachment index!");

		return mColorAttachments[index];
	}

	const FramebufferDesc& OpenGLFramebuffer::GetDesc() const
	{
		return m_Desc;
	}

	void OpenGLFramebuffer::Delete()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
		glDeleteTextures(1, &mDepthAttachment);
	}

	void OpenGLFramebuffer::AttachColorTexture(U32 id, int samples, U32 format, U32 width, U32 height, U32 index)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	}

	void OpenGLFramebuffer::AttachDepthTexture(U32 id, int samples, U32 format, U32 attachmentType, U32 width, U32 height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	}

	bool OpenGLFramebuffer::IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case Dominion::FramebufferTextureFormat::DEPTH24STENCIL8:
			return true;
		}

		return false;
	}

	void OpenGLFramebuffer::CreateTextures(bool multisampled, U32* outId, U32 count)
	{
		glCreateTextures(TextureTarget(multisampled), count, outId);
	}

	GLenum OpenGLFramebuffer::TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	void OpenGLFramebuffer::BindTexture(bool multisampled, U32 id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}

}
