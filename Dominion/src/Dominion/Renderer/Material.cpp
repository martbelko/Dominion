#include "dmpch.h"
#include "Material.h"

#include "Dominion/Renderer/Shader.h"

namespace Dominion {

	void Material::UploadAllShaderUniforms() const
	{
		for (auto& [name, value] : m_IntHashmap)
			m_Shader->SetInt(name, value);
		for (auto& [name, value] : m_FloatHashmap)
			m_Shader->SetFloat(name, value);
		for (auto& [name, value] : m_Vec2Hashmap)
			m_Shader->SetFloat2(name, value);
		for (auto& [name, value] : m_Vec3Hashmap)
			m_Shader->SetFloat3(name, value);
		for (auto& [name, value] : m_Vec4Hashmap)
			m_Shader->SetFloat4(name, value);
		for (auto& [name, value] : m_Mat2Hashmap)
			m_Shader->SetMat2(name, value);
		for (auto& [name, value] : m_Mat3Hashmap)
			m_Shader->SetMat3(name, value);
		for (auto& [name, value] : m_Mat4Hashmap)
			m_Shader->SetMat4(name, value);
	}

}
