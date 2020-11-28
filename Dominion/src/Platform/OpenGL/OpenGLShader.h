#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Shader.h"

#include <glm/glm.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

namespace Dominion {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string_view& name, const std::string_view& vertexFilepath, const std::string_view& pixelFilepath);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;

		virtual void SetInt(const std::string_view& name, int value) override;
		virtual void SetIntArray(const std::string_view& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string_view& name, float value) override;
		virtual void SetFloat2(const std::string_view& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string_view& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string_view& name, const glm::vec4& value) override;
		virtual void SetMat2(const std::string_view& name, const glm::mat2& matrix) override;
		virtual void SetMat3(const std::string_view& name, const glm::mat3& matrix) override;
		virtual void SetMat4(const std::string_view& name, const glm::mat4& value) override;
	private:
		bool ReadFile(const std::string_view& filepath, std::string& output);
		void Compile(std::unordered_map<uint32_t, std::string> sources);
	private:
		uint32_t m_RendererID;
	};

}
