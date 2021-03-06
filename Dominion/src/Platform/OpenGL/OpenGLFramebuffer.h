#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Framebuffer.h"

namespace Dominion {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferDesc& desc);
		virtual ~OpenGLFramebuffer() override;

		void Invalidate();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override;

		virtual const FramebufferDesc& GetDesc() const override;
	private:
		void Delete();
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachmentID, m_DepthStencilAttachmentID;
		FramebufferDesc m_Desc;
	};

}
