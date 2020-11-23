#include "dmpch.h"
#include "OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

namespace Dominion {

	OpenGLShader::OpenGLShader(const std::string_view& name, const std::string_view& vertexFilepath, const std::string_view& pixelFilepath)
		: Shader(name)
	{
		DM_PROFILE_FUNCTION();

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
		DM_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		DM_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::SetInt(const std::string_view& name, int value)
	{
		DM_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.data());
	#if DM_ENABLE_CORE_LOGGER
		if (location == -1)
			DM_CORE_WARN("Uniform '{0}' was not found!", name);
	#endif
		glUniform1i(location, value);
	}

	void OpenGLShader::SetIntArray(const std::string_view& name, int* values, uint32_t count)
	{
		DM_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.data());
	#if DM_ENABLE_CORE_LOGGER
		if (location == -1)
			DM_CORE_WARN("Uniform '{0}' was not found!", name);
	#endif
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetFloat(const std::string_view& name, float value)
	{
		DM_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.data());
	#if DM_ENABLE_CORE_LOGGER
		if (location == -1)
			DM_CORE_WARN("Uniform '{0}' was not found!", name);
	#endif
		glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string_view& name, const glm::vec2& value)
	{
		DM_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.data());
	#if DM_ENABLE_CORE_LOGGER
		if (location == -1)
			DM_CORE_WARN("Uniform '{0}' was not found!", name);
	#endif
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string_view& name, const glm::vec3& value)
	{
		DM_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.data());
	#if DM_ENABLE_CORE_LOGGER
		if (location == -1)
			DM_CORE_WARN("Uniform '{0}' was not found!", name);
	#endif
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string_view& name, const glm::vec4& value)
	{
		DM_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.data());
	#if DM_ENABLE_CORE_LOGGER
		if (location == -1)
			DM_CORE_WARN("Uniform '{0}' was not found!", name);
	#endif
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat2(const std::string_view& name, const glm::mat2& matrix)
	{
		DM_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.data());
	#if DM_ENABLE_CORE_LOGGER
		if (location == -1)
			DM_CORE_WARN("Uniform '{0}' was not found!", name);
	#endif
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMat3(const std::string_view& name, const glm::mat3& matrix)
	{
		DM_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.data());
	#if DM_ENABLE_CORE_LOGGER
		if (location == -1)
			DM_CORE_WARN("Uniform '{0}' was not found!", name);
	#endif
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMat4(const std::string_view& name, const glm::mat4& matrix)
	{
		DM_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.data());
	#if DM_ENABLE_CORE_LOGGER
		if (location == -1)
			DM_CORE_WARN("Uniform '{0}' was not found!", name);
	#endif
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	bool OpenGLShader::ReadFile(const std::string_view& filepath, std::string& output)
	{
		DM_PROFILE_FUNCTION();

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
		DM_PROFILE_FUNCTION();

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
