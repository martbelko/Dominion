#include "dmpch.h"
#include "Renderer.h"

#include "Dominion/Renderer/SimpleRenderer2D.h"
#include "Dominion/Renderer/Mesh.h"
#include "Dominion/Renderer/Material.h"
#include "Dominion/Renderer/Model.h"

namespace Dominion {

	glm::mat4 Renderer::m_ProjectionViewMatrix = glm::mat4(1.0f);

	void Renderer::Init()
	{
		DM_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
		SimpleRenderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		DM_PROFILE_FUNCTION();

		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		m_ProjectionViewMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::BeginScene(const PerspectiveCamera& camera)
	{
		m_ProjectionViewMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Pipeline>& pipeline, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_ProjectionViewMatrix);
		shader->SetMat4("u_Transform", transform);
		pipeline->Bind();

		if (pipeline->GetIndexBuffer())
			RenderCommand::DrawIndexed(pipeline);
		else
		{
			uint32_t count = pipeline->GetVertexBuffer()->GetSize() / pipeline->GetInputLayout()->GetStride();
			RenderCommand::Draw(pipeline, count);
		}
	}

	void Renderer::Submit(const Mesh& mesh, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		U32F diffuseNr = 1;
		U32F specularNr = 1;
		U32F normalNr = 1;
		U32F heightNr = 1;

		const std::vector<MeshTexture>& meshTextures = mesh.GetTextures();
		for (U32F i = 0; i < meshTextures.size(); ++i)
		{
			std::string number;
			const std::string& name = meshTextures[i].Type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

			std::string fullname = name + number;
			if (shader->HasUniform(fullname))
				shader->SetInt(fullname, i);

			meshTextures[i].Texture->Bind(i);
		}

		Dominion::Renderer::Submit(shader, mesh.GetPipeline());
	}

	void Renderer::Submit(const Ref<Model>& model, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		const std::vector<Mesh> meshes = model->GetMeshes();

		shader->Bind();

		shader->SetMat4("u_ViewProjection", m_ProjectionViewMatrix);
		shader->SetMat4("u_Transform", transform);

		for (const Mesh& mesh : meshes)
		{
			Renderer::Submit(mesh, shader, transform);
		}
	}

}
