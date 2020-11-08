#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>


template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func)
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		float duration = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(endTimePoint - m_StartTimePoint).count()) * 0.000001f;
		m_Stopped = true;
		m_Func({ m_Name, duration });
	}
private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
	bool m_Stopped = false;
	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfilerResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
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
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	// Update
	{
		PROFILE_SCOPE("Camera update");
		m_Camera.OnUpdate(timestep);
		m_Camera.Refresh();
	}

	// Render
	{
		PROFILE_SCOPE("Render prep");
		Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Dominion::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Render draw");
		Dominion::Renderer2D::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		float xPos = 0.0f, yPos = 0.0f;
		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(xPos, yPos, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Dominion::Renderer2D::DrawQuad({ xPos, yPos }, { 0.1f, 0.1f }, { 0.8f, 0.2f, 0.3f, 1.0f });
				xPos += 0.11f;
			}
			yPos += 0.11f;
			xPos = 0.0f;
		}

		Dominion::Renderer2D::DrawQuad(glm::mat4(1.0f), m_Texture2D);
		Dominion::Renderer2D::DrawQuad(glm::mat4(1.0f), m_TestTexture);

		Dominion::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(Dominion::Event& e)
{
	PROFILE_SCOPE("Sandbox2D::OnEvent");
	m_Camera.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
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

	ImGui::Begin("Profiler");

	for (auto& result : m_ProfilerResults)
	{
		ImGui::Text("%.3fms %s", result.time, result.Name);
	}

	ImGui::End();

	m_ProfilerResults.clear();
}
