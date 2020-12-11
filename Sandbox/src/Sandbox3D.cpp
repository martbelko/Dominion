#include "Sandbox3D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox3D::Sandbox3D()
	: Dominion::Layer("Sandbox3D") {}

void Sandbox3D::OnAttach()
{
	/* Disable cursor */
	Dominion::Application::Get().GetWindow().ShowCursor(false);

	/* Setup camera */
	float wHeight = static_cast<float>(Dominion::Application::Get().GetWindow().GetHeight());
	float wWidth = static_cast<float>(Dominion::Application::Get().GetWindow().GetWidth());
	float ratio = wWidth / wHeight;

	m_Camera = Dominion::PerspectiveCameraController(ratio, false);
	m_Camera.GetCamera().SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));

	/* Setup 3D Render stuff */
	float vertices[] = {
}

void Sandbox3D::OnUpdate(const Dominion::Timestep& timestep)
{
	m_Camera.OnUpdate(timestep);
	Dominion::Renderer2D::ResetStats();

	Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Dominion::RenderCommand::ClearColorBuffer();

	Dominion::Renderer::BeginScene(m_Camera.GetCamera());

	ourShader->Bind();
	ourShader->SetMat4("u_ViewProjection", m_Camera.GetCamera().GetViewProjectionMatrix());

	ourModel->Draw(ourShader);

	Dominion::Renderer::EndScene();
}

void Sandbox3D::OnEvent(Dominion::Event& e)
{
	m_Camera.OnEvent(e);
	e.Dispatch<Dominion::KeyPressedEvent>(DM_BIND_EVENT_FN(Sandbox3D::OnKeyPressed));
}

bool Sandbox3D::OnKeyPressed(Dominion::KeyPressedEvent& e)
{
	return false;
}
