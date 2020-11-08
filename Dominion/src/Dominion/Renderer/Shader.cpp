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

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		DM_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string_view& name, const std::string_view& vertexFilepath, const std::string_view& pixelFilepath)
	{
		Ref<Shader> shader = Shader::Create(name, vertexFilepath, pixelFilepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		DM_CORE_ASSERT(Exists(name), "Shader does not exists!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}
