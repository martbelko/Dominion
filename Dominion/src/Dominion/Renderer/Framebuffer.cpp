#include "dmpch.h"
#include "Dominion/Renderer/Framebuffer.h"

#include "Dominion/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Dominion {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

