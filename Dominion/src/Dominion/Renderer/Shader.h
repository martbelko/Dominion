#pragma once

#include "Dominion/Core/Base.h"
#include "Bindable.h"

#include <glm/glm.hpp>

#include <string>
#include <string_view>

namespace Dominion {

	class Shader : public Bindable
	{
	public:
		Shader(const std::string_view& name)
			: m_Name(name) {}
		virtual ~Shader() = default;

		virtual const std::string& GetName() const { return m_Name; }

		virtual void SetInt(const std::string_view& name, int value) = 0;
		virtual void SetIntArray(const std::string_view& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string_view& name, float value) = 0;
		virtual void SetFloat2(const std::string_view& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string_view& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string_view& name, const glm::vec4& value) = 0;
		virtual void SetMat2(const std::string_view& name, const glm::mat2& matrix) = 0;
		virtual void SetMat3(const std::string_view& name, const glm::mat3& matrix) = 0;
		virtual void SetMat4(const std::string_view& name, const glm::mat4& value) = 0;

		static Ref<Shader> Create(const std::string_view& name, const std::string_view& vertexFilepath, const std::string_view& pixelFilepath);
	protected:
		const std::string m_Name;
	};

}
