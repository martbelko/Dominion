#include "dmpch.h"
#include "Renderer.h"

#include "Dominion/Renderer/SimpleRenderer2D.h"

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

}
