#include "dmpch.h"
#include "Dominion/Renderer/Shader.h"

#include "Dominion/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Dominion {

	Ref<Shader> Shader::Create(const std::filesystem::path& vertexFilepath, const std::filesystem::path& fragmentFilepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(vertexFilepath, fragmentFilepath);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		DM_CORE_ASSERT(!Exists(name), "Shader already exists!");
		mShaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::filesystem::path& vertexFilepath, const std::filesystem::path& fragmentFilepath)
	{
		auto shader = Shader::Create(vertexFilepath, fragmentFilepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::filesystem::path& vertexFilepath, const std::filesystem::path& fragmentFilepath)
	{
		auto shader = Shader::Create(vertexFilepath, fragmentFilepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		DM_CORE_ASSERT(Exists(name), "Shader not found!");
		return mShaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return mShaders.find(name) != mShaders.end();
	}

}
