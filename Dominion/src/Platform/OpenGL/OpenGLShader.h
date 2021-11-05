#pragma once

#include <filesystem>

#include <glm/glm.hpp>

#include "Dominion/Renderer/Shader.h"

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace Dominion {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::filesystem::path& vertexFilepath, const std::filesystem::path& fragmentFilepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual const std::string& GetName() const override { return mName; }

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);
	private:
		static std::string ReadFile(const std::filesystem::path& filepath);
	private:
		uint32_t mRendererID;
		std::string mName;

		std::filesystem::path mVertexFilepath;
		std::filesystem::path mFragmentFilepath;

		std::unordered_map<GLenum, std::vector<uint32_t>> mVulkanSPIRV;
		std::unordered_map<GLenum, std::vector<uint32_t>> mOpenGLSPIRV;

		std::unordered_map<GLenum, std::string> mOpenGLSourceCode;
	};

}
