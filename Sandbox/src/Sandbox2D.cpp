#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
	: Dominion::Layer("Sandbox2D")
{
	m_Texture2D = Dominion::Texture2D::Create("assets/Textures/TestTexture.jpg");
	m_TestTexture = Dominion::Texture2D::Create("assets/Textures/unnamed.png");

	/* Setup camera */
	float wHeight = static_cast<float>(Dominion::Application::Get().GetWindow().GetHeight());
	float wWidth = static_cast<float>(Dominion::Application::Get().GetWindow().GetWidth());
	float ratio = wWidth / wHeight;
	m_Camera = Dominion::OrthographicCamera(ratio, 1.0f);
}

void Sandbox2D::OnUpdate(const Dominion::Timestep& timestep)
{
	DM_PROFILE_FUNCTION();

	static int sign = -1;
	float t = timestep * sign * 50.0f;;
	m_Rotation += timestep * sign * 50.0f;
	m_Rotation = std::clamp(m_Rotation, -180.0f, 180.0f);
	if (m_Rotation == -180.0f)
		sign = 1;
	else if (m_Rotation == 180.0f)
		sign = -1;

	// Update
	m_Camera.OnUpdate(timestep);
	m_Camera.Refresh();

	// Render
	{
		DM_PROFILE_SCOPE("Render Prep");
		Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Dominion::RenderCommand::Clear();
	}

	{
		DM_PROFILE_SCOPE("Render Draw");
		Dominion::Renderer2D::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		float xPos = 0.0f, yPos = 0.0f;
		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(xPos, yPos, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Dominion::Renderer2D::DrawRotatedQuad({ xPos, yPos }, { 0.1f, 0.1f }, glm::radians(m_Rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
				xPos += 0.11f;
			}
			yPos += 0.11f;
			xPos = 0.0f;
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
	/*static bool show = true;
	ImGui::ShowDemoWindow(&show);*/
	ImGui::Begin("Camera Control");
	glm::vec3& pos = m_Camera.GetPosition();
	ImGui::DragFloat("X", &pos.x, 0.01f, -5.0f, 5.0f);
	ImGui::DragFloat("Y", &pos.y, 0.01f, -5.0f, 5.0f);
	ImGui::DragFloat("Z", &pos.z, 0.01f, -5.0f, 5.0f);

	if (ImGui::Button("Reset Position"))
		pos = glm::vec3(0.0f, 0.0f, 0.0f);

	ImGui::End();
}
