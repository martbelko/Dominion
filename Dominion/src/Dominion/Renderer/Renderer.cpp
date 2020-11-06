#include "dmpch.h"
#include "Renderer.h"

namespace Dominion {

	glm::mat4 Renderer::m_ProjectionViewMatrix = glm::mat4(1.0f);

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera)
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

		RenderCommand::DrawIndexed(pipeline);
	}

}
