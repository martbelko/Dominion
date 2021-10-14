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

		virtual void Resize(U32 width, U32 height) override;

		virtual U32 GetColorAttachmentRendererID(U32 index = 0) const override;

		virtual const FramebufferDesc& GetDesc() const override;
	private:
		void Delete();
	private:
		static void AttachColorTexture(U32 id, int samples, U32 format, U32 width, U32 height, U32 index);
		static void AttachDepthTexture(U32 id, int samples, U32 format, U32 attachmentType, U32 width, U32 height);

		static bool IsDepthFormat(FramebufferTextureFormat format);
		static void CreateTextures(bool multisampled, U32* outId, U32 count);
		static U32 TextureTarget(bool multisampled);
		static void BindTexture(bool multisampled, U32 id);
	private:
		U32 m_RendererID;
		FramebufferDesc m_Desc;

		std::vector<FramebufferTextureDesc> mColorAttachmentSpecs;
		FramebufferTextureDesc mDepthAttachmentSpec = FramebufferTextureFormat::None;

		std::vector<U32> mColorAttachments;
		U32 mDepthAttachment;
	};

}
