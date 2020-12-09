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
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
	};

	Dominion::Ref<Dominion::InputLayout> il = Dominion::InputLayout::Create(
		{
			{ "Position", Dominion::DataType::Float3 },
			{ "Color", Dominion::DataType::Float4 }
		});

	Dominion::Ref<Dominion::Shader> shader = Dominion::Shader::Create("CubeShader", "assets/Shaders/3DVS.glsl", "assets/Shaders/3DPS.glsl");

	Dominion::Ref<Dominion::Mesh> mesh = Dominion::Mesh::Create(vertices, sizeof(vertices), il);
	Dominion::Ref<Dominion::Material> material = Dominion::Material::Create(shader);
	material->SetUniform("u_Color", glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	material->SetUniform("u_Vec", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Model = Dominion::Model::Create(mesh, material);
}

void Sandbox3D::OnUpdate(const Dominion::Timestep& timestep)
{
	m_Camera.OnUpdate(timestep);
	Dominion::Renderer2D::ResetStats();

	Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Dominion::RenderCommand::ClearColorBuffer();

	Dominion::Renderer::BeginScene(m_Camera.GetCamera());

	static float t = timestep;
	t += timestep * 50.0f;

	glm::mat4 transform;
	for (int i = 0; i < 20; ++i)
	{
		transform = glm::translate(glm::mat4(1.0f), glm::vec3(2 * i, i, glm::sin(glm::radians(t))));
		Dominion::Renderer::Submit(m_Model, transform);
	}

	Dominion::Renderer::EndScene();
}

void Sandbox3D::OnEvent(Dominion::Event& e)
{
	m_Camera.OnEvent(e);
	e.Dispatch<Dominion::KeyPressedEvent>(DM_BIND_EVENT_FN(Sandbox3D::OnKeyPressed));
}

bool Sandbox3D::OnKeyPressed(Dominion::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == Dominion::Key::Escape)
		m_ShowCursor = !m_ShowCursor;

	return false;
}
