#include "dmpch.h"
#include "Renderer.h"

void Dominion::Renderer::Init()
{
	RenderCommand::Init();
	Renderer2D::Init();
}

void Dominion::Renderer::Shutdown()
{
	Renderer2D::Shutdown();
}

void Dominion::Renderer::BeginScene()
{
}

void Dominion::Renderer::EndScene()
{
}

void Dominion::Renderer::Submit(const Ref<Shader>& shader, const Ref<Pipeline>& pipeline)
{
	shader->Bind();
	pipeline->Bind();

	RenderCommand::DrawIndexed(pipeline);
}
