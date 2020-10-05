#include "dmpch.h"
#include "Renderer.h"

namespace Dominion {

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::BeginScene()
	{
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
