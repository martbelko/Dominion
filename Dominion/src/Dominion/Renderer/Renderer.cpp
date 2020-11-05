#include "dmpch.h"
#include "Renderer.h"

namespace Dominion {

	glm::mat4& Renderer::m_ProjectionMatrix = glm::mat4(1.0f);

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		m_ProjectionMatrix = camera.GetProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Pipeline>& pipeline)
	{
		shader->Bind();
		pipeline->Bind();

		RenderCommand::DrawIndexed(pipeline);
	}

}
