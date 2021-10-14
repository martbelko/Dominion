#pragma once

#include "Dominion/Core/Base.h"

#include "Dominion/Renderer/Bindable.h"

namespace Dominion {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,

		// Depth/Stencil
		DEPTH24STENCIL8
	};

	struct FramebufferTextureDesc
	{
		FramebufferTextureDesc() = default;
		FramebufferTextureDesc(FramebufferTextureFormat textureFormat)
			: textureFormat(textureFormat) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentDesc
	{
		FramebufferAttachmentDesc() = default;
		FramebufferAttachmentDesc(std::initializer_list<FramebufferTextureDesc> attachments)
			: attachments(attachments) {}

		std::vector<FramebufferTextureDesc> attachments;
	};

	struct FramebufferDesc
	{
		uint32_t width = 0, height = 0;
		FramebufferAttachmentDesc attachments;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer : public Bindable
	{
	public:
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID(U32 index = 0) const = 0;

		virtual const FramebufferDesc& GetDesc() const = 0;

		static Ref<Framebuffer> Create(const FramebufferDesc& desc);
	};

}
