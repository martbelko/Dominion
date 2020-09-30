#pragma once

#include "Dominion/Renderer/Shader.h"

#include <string>
#include <string_view>
#include <unordered_map>

namespace Dominion {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string_view& name, const std::string_view& vertexFilepath, const std::string_view& pixelFilepath);
		virtual ~OpenGLShader() override;

		virtual void Bind() override;
	private:
		bool ReadFile(const std::string_view& filepath, std::string& output);
		void Compile(std::unordered_map<unsigned int, std::string> sources);
	private:
		unsigned int m_RendererID;
	};

}
