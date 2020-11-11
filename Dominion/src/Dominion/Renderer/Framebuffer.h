#pragma once

#include "Dominion/Core/Base.h"

#include "Dominion/Renderer/Bindable.h"

namespace Dominion {

	struct FramebufferDesc
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer : public Bindable
	{
	public:
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferDesc& GetDesc() const = 0;

		static Ref<Framebuffer> Create(const FramebufferDesc& desc);
	};

}
