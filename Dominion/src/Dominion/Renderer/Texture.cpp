#include "dmpch.h"
#include "Texture.h"

#include "Dominion/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Dominion {

	Dominion::Ref<Dominion::Texture2D> Texture2D::Create(const std::string_view& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(filepath);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
