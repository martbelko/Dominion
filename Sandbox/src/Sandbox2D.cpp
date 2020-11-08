#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
{
	float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_Pipeline = Dominion::Pipeline::Create(Dominion::VertexBuffer::Create(vertices, sizeof(vertices)), Dominion::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)), Dominion::InputLayout::Create(
		{
			{ "Position", Dominion::DataType::Float3 },
			{ "TexCoord", Dominion::DataType::Float2 }
		}
	));

	m_ShaderLibrary.Load("Test", "assets/Shaders/TestVS.glsl", "assets/Shaders/TestPS.glsl");

	m_ShaderLibrary.Load("Texture", "assets/Shaders/TextureVS.glsl", "assets/Shaders/TexturePS.glsl");

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
	m_Camera.OnUpdate(timestep);
	m_Camera.Refresh();

	Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Dominion::RenderCommand::Clear();

	Dominion::Renderer::BeginScene(m_Camera);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	float xPos = 0.0f, yPos = 0.0f;
	for (int y = 0; y < 20; ++y)
	{
		for (int x = 0; x < 20; ++x)
		{
			glm::vec3 pos(xPos, yPos, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Dominion::Renderer::Submit(m_ShaderLibrary.Get("Test"), m_Pipeline, transform);
			xPos += 0.11f;
		}
		yPos += 0.11f;
		xPos = 0.0f;
	}

	m_Texture2D->Bind();

	auto textureShader = m_ShaderLibrary.Get("Texture");
	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);
	Dominion::Renderer::Submit(textureShader, m_Pipeline);
	m_TestTexture->Bind();
	Dominion::Renderer::Submit(textureShader, m_Pipeline);

	Dominion::Renderer::EndScene();
}

void Sandbox2D::OnEvent(Dominion::Event& e)
{
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
}
