#pragma once

#include <Dominion.h>
#include <unordered_map>

using namespace Dominion;

class Material
{
public:
	Material() = default;
	Material(const Ref<Shader>& shader)
		: m_Shader(shader) {}

	const Ref<Shader>& GetShader() const { return m_Shader; }

	void UploadAllShaderUniforms() const
	{
		m_Shader->Bind();

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

	void SetInt(const std::string_view& name, int value)
	{
		m_IntHashmap[std::string(name)] = value;
	}

	void SetFloat(const std::string_view& name, float value)
	{
		m_FloatHashmap[std::string(name)] = value;
	}

	void SetFloat2(const std::string_view& name, const glm::vec2& value)
	{
		m_Vec2Hashmap[std::string(name)] = value;
	}

	void SetFloat3(const std::string_view& name, const glm::vec3& value)
	{
		m_Vec3Hashmap[std::string(name)] = value;
	}

	void SetFloat4(const std::string_view& name, const glm::vec4& value)
	{
		m_Vec4Hashmap[std::string(name)] = value;
	}

	void SetMat2(const std::string_view& name, const glm::mat2& value)
	{
		m_Mat2Hashmap[std::string(name)] = value;
	}

	void SetMat3(const std::string_view& name, const glm::mat3& value)
	{
		m_Mat3Hashmap[std::string(name)] = value;
	}

	void SetMat4(const std::string_view& name, const glm::mat4& value)
	{
		m_Mat4Hashmap[std::string(name)] = value;
	}


	static Ref<Material> Create(const Ref<Shader>& shader)
	{
		return CreateRef<Material>(shader);
	}
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

class Mesh
{
public:
	Mesh(const float* vertices, uint32_t size, const Ref<InputLayout>& layout)
	{
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, size);

		m_Pipeline = Pipeline::Create(vb, layout);
	}

	const Ref<Pipeline>& GetPipeline() const { return m_Pipeline; }

	static Ref<Mesh> Create(const float* vertices, uint32_t size, const Ref<InputLayout>& layout)
	{
		return CreateRef<Mesh>(vertices, size, layout);
	}
private:
	Ref<Pipeline> m_Pipeline;
};

class Model
{
public:
	Model(const Ref<Mesh>& mesh, const Ref<Material>& material)
		: m_Mesh(mesh), m_Material(material) {}

	const Ref<Mesh>& GetMesh() const { return m_Mesh; }
	const Ref<Material>& GetMaterial() const { return m_Material; }

	static Ref<Model> Create(const Ref<Mesh>& mesh, const Ref<Material>& material)
	{
		return CreateRef<Model>(mesh, material);
	}
private:
	Ref<Mesh> m_Mesh;
	Ref<Material> m_Material;
};

static void RenderModel(const Ref<Model>& model)
{
	Renderer::Submit(model->GetMaterial()->GetShader(), model->GetMesh()->GetPipeline());
}

static void RenderModel(const Ref<Model>& model, const glm::mat4& transform)
{
	model->GetMaterial()->UploadAllShaderUniforms();
	Renderer::Submit(model->GetMaterial()->GetShader(), model->GetMesh()->GetPipeline(), transform);
}


class Sandbox2D : public Dominion::Layer
{
public:
	Sandbox2D();
	virtual void OnAttach() override;
	virtual void OnUpdate(const Dominion::Timestep& timestep) override;
	virtual void OnEvent(Dominion::Event& e) override;
	virtual void OnImGuiRender() override;
private:
	bool OnKeyPressed(Dominion::KeyPressedEvent& e);
private:
	/*Dominion::Ref<Dominion::Pipeline> m_3DPipeline;
	Dominion::Ref<Dominion::Shader> m_3DShader;*/
	Ref<Model> m_Model;

	Dominion::Ref<Dominion::Texture2D> m_Texture2D;
	Dominion::Ref<Dominion::Texture2D> m_TestTexture;

	Dominion::PerspectiveCameraController m_Camera;

	bool m_ShowCursor = false;

	float m_Rotation = 0.0f;
	float m_RotationSpeed = 40.0f;

	int m_Count = 10;
};
