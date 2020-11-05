#include "dmpch.h"
#include "Shader.h"

#include "Dominion/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Dominion {

	Ref<Shader> Shader::Create(const std::string_view& name, const std::string_view& vertexFilepath, const std::string_view& pixelFilepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(name, vertexFilepath, pixelFilepath);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
