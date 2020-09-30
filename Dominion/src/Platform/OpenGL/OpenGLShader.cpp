#include "dmpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

namespace Dominion {

	OpenGLShader::OpenGLShader(const std::string_view& name, const std::string_view& vertexFilepath, const std::string_view& pixelFilepath)
		: Shader(name)
	{
		std::string vertexSrc;
		ReadFile(vertexFilepath, vertexSrc);
		std::string pixelSrc;
		ReadFile(pixelFilepath, pixelSrc);

		std::unordered_map<unsigned int, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = pixelSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	bool OpenGLShader::ReadFile(const std::string_view& filepath, std::string& output)
	{
		std::ifstream file(filepath.data(), std::ios::in | std::ios::binary);
		if (file)
		{
			file.seekg(0, std::ios::end);
			size_t size = file.tellg();
			if (size != -1)
			{
				output.resize(size);
				file.seekg(0, std::ios::beg);
				file.read(output.data(), size);

				DM_CORE_TRACE("Reading from file '{0}' successful", filepath);

				return true;
			}
			else
			{
				DM_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			DM_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return false;
	}

	void OpenGLShader::Compile(std::unordered_map<unsigned int, std::string> sources)
	{
		unsigned int program = glCreateProgram();
		DM_CORE_ASSERT(sources.size() <= 2, "Engine only supports 2 shaders for now!");

		std::array<unsigned int, 2> shaders;
		unsigned int shaderIndex = 0;
		for (auto& kv : sources)
		{
			unsigned int type = kv.first;
			const std::string& source = kv.second;

			unsigned int shader = glCreateShader(type);

			const char* sourceStr = source.c_str();
			glShaderSource(shader, 1, &sourceStr, 0);

			glCompileShader(shader);

			int isCompiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (!isCompiled)
			{
				int len;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

				std::string info;
				info.resize(len);
				glGetShaderInfoLog(shader, len, &len, info.data());

				glDeleteShader(shader);

				DM_CORE_ERROR(info);
				DM_CORE_ASSERT(false, "Shader compilation failure!");
			}

			glAttachShader(program, shader);
			shaders[shaderIndex++] = shader;
		}

		DM_CORE_TRACE("Shader compilation successful");

		glLinkProgram(program);

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (!isLinked)
		{
			int len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

			std::string info;
			info.resize(len);
			glGetProgramInfoLog(program, len, &len, info.data());

			glDeleteProgram(program);

			for (unsigned int shader : shaders)
				glDeleteShader(shader);

			DM_CORE_ERROR(info);
			DM_CORE_ASSERT(false, "Shader link failure!");
		}

		DM_CORE_TRACE("Shader linked successfully");

		for (unsigned int shader : shaders)
		{
			glDetachShader(program, shader);
			glDeleteShader(shader);
		}

		m_RendererID = program;
	}

}
