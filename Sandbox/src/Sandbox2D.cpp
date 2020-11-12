#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
	: Dominion::Layer("Sandbox2D") {}

void Sandbox2D::OnAttach()
{
	m_Texture2D = Dominion::Texture2D::Create("assets/Textures/TestTexture.jpg");
	m_TestTexture = Dominion::Texture2D::Create("assets/Textures/unnamed.png");

	/* Setup camera */
	float wHeight = static_cast<float>(Dominion::Application::Get().GetWindow().GetHeight());
	float wWidth = static_cast<float>(Dominion::Application::Get().GetWindow().GetWidth());
	float ratio = wWidth / wHeight;
	m_Camera = Dominion::OrthographicCameraController(ratio, 1.0f);

	Dominion::FramebufferDesc desc;
	desc.Width = Dominion::Application::Get().GetWindow().GetWidth();
	desc.Height = Dominion::Application::Get().GetWindow().GetHeight();
}

void Sandbox2D::OnUpdate(const Dominion::Timestep& timestep)
{
	DM_PROFILE_FUNCTION();

	static int sign = -1;
	m_Rotation += timestep * sign * m_RotationSpeed;
	m_Rotation = std::clamp(m_Rotation, -180.0f, 180.0f);
	if (m_Rotation == -180.0f)
		sign = 1;
	else if (m_Rotation == 180.0f)
		sign = -1;

	// Update
	m_Camera.OnUpdate(timestep);

	// Render
	{
		DM_PROFILE_SCOPE("Render Prep");
		Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Dominion::RenderCommand::Clear();
	}

	{
		DM_PROFILE_SCOPE("Render Draw");
		Dominion::Renderer2D::BeginScene(m_Camera.GetCamera());

		float cap = m_Count / 2.0f;
		for (float y = -cap; y < cap; y += 0.5f)
		{
			for (float x = -cap; x < cap; x += 0.5f)
			{
				glm::vec4 color = { (x + cap) / static_cast<float>(m_Count),
					0.3f,
					(y + cap) / static_cast<float>(m_Count),
					1.0f };
				Dominion::Renderer2D::DrawRotatedQuad({ x, y }, { 0.45f, 0.45f }, glm::radians(m_Rotation), color);
			}
		}

		Dominion::Renderer2D::DrawQuad(glm::mat4(1.0f), m_Texture2D);
		Dominion::Renderer2D::DrawQuad(glm::mat4(1.0f), m_TestTexture, 5.0f);

		Dominion::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(Dominion::Event& e)
{
	DM_PROFILE_FUNCTION();
	m_Camera.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	DM_PROFILE_FUNCTION();

	ImGui::Begin("Camera Control");

	ImGui::SliderFloat("Rotation speed", &m_RotationSpeed, 0.0f, 500.0f);
	ImGui::SliderInt("Number of Quads", &m_Count, 0, 100);

	ImGui::End();

	const auto& stats = Dominion::Renderer2D::GetStats();
	ImGui::Begin("Renderer2D Stats");
	ImGui::Text("Draw calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();

	Dominion::Renderer2D::ResetStats();
}
