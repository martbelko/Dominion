#pragma once

#include "Dominion/Core/Base.h"

#include <glm/glm.hpp>

namespace Dominion {

	// Forward declarations
	class Shader;

	class Material
	{
	public:
		Material() = default;
		Material(const Ref<Shader>& shader)
			: m_Shader(shader) {}

		const Ref<Shader>& GetShader() const { return m_Shader; }

		void UploadAllShaderUniforms() const;

		void SetUniform(const std::string_view& name, int value) { m_IntHashmap[std::string(name)] = value; }
		void SetUniform(const std::string_view& name, float value) {m_FloatHashmap[std::string(name)] = value; }
		void SetUniform(const std::string_view& name, const glm::vec2& value) { m_Vec2Hashmap[std::string(name)] = value; }
		void SetUniform(const std::string_view& name, const glm::vec3& value) { m_Vec3Hashmap[std::string(name)] = value; }
		void SetUniform(const std::string_view& name, const glm::vec4& value) { m_Vec4Hashmap[std::string(name)] = value; }
		void SetUniform(const std::string_view& name, const glm::mat2& value) { m_Mat2Hashmap[std::string(name)] = value; }
		void SetUniform(const std::string_view& name, const glm::mat3& value) { m_Mat3Hashmap[std::string(name)] = value; }
		void SetUniform(const std::string_view& name, const glm::mat4& value) { m_Mat4Hashmap[std::string(name)] = value; }

		static Ref<Material> Create(const Ref<Shader>& shader) { return CreateRef<Material>(shader); }
	private:
		Ref<Shader> m_Shader;
		std::unordered_map<std::string, int> m_IntHashmap;
		std::unordered_map<std::string, float> m_FloatHashmap;
		std::unordered_map<std::string, glm::vec2> m_Vec2Hashmap;
		std::unordered_map<std::string, glm::vec3> m_Vec3Hashmap;
		std::unordered_map<std::string, glm::vec4> m_Vec4Hashmap;
		std::unordered_map<std::string, glm::mat2> m_Mat2Hashmap;
		std::unordered_map<std::string, glm::mat3> m_Mat3Hashmap;
		std::unordered_map<std::string, glm::mat4> m_Mat4Hashmap;
		// std::unordered_map<std::string, int*> m_IntArrayHashmap;
	};

}
