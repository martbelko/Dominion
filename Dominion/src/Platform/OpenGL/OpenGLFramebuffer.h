#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Framebuffer.h"

namespace Dominion {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferDesc& desc);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetColorAttachmentRendererID() const override;

		virtual const FramebufferDesc& GetDesc() const override;
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachmentID, m_DepthStencilAttachmentID;
		FramebufferDesc m_Desc;
	};

}
