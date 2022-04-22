#include "dmpch.h"
#include "Dominion/Renderer/Texture.h"

#include "Dominion/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Dominion {

	Ref<Texture2D> Texture2D::Create(const Texture2DSpecification& textureSpecification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(textureSpecification);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
