#include <Dominion.h>

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Dominion::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_Pipeline = Dominion::Pipeline::Create(Dominion::VertexBuffer::Create(vertices, sizeof(vertices)), Dominion::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)), Dominion::InputLayout::Create(
			{
				{ "Position", Dominion::DataType::Float3 }
			}
		));

		m_Shader = Dominion::Shader::Create("Test", "TestVS.glsl", "TestPS.glsl");
	}

	void OnUpdate(const Dominion::Timestep& timestep) override
	{
		m_Camera.SetPosition(m_CameraPos);

		Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Dominion::RenderCommand::Clear();

		Dominion::Renderer::BeginScene(m_Camera);
		Dominion::Renderer::Submit(m_Shader, m_Pipeline);
		Dominion::Renderer::EndScene();
	}

	void OnEvent(Dominion::Event& e) override
	{

	}

	void OnImGuiRender() override
	{
		/*static bool show = true;
		ImGui::ShowDemoWindow(&show);*/
		ImGui::Begin("Camera Control");
		ImGui::DragFloat("X", &m_CameraPos.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat("Y", &m_CameraPos.y, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat("Z", &m_CameraPos.z, 0.01f, -5.0f, 5.0f);

		if (ImGui::Button("Reset Position"))
		{
			m_CameraPos[0] = 0.0f;
			m_CameraPos[1] = 0.0f;
			m_CameraPos[2] = 0.0f;
		}

		ImGui::End();
	}
private:
	Dominion::Ref<Dominion::Pipeline> m_Pipeline;
	Dominion::Ref<Dominion::Shader> m_Shader;

	glm::vec3 m_CameraPos = { 0.0f, 0.0f, 0.0f };

	Dominion::OrthographicCamera m_Camera = Dominion::OrthographicCamera(-1.0f, 1.0f, -1.0f, 1.0f);
};

class Sandbox : public Dominion::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Dominion::Application* Dominion::CreateApplication()
{
	return new Sandbox();
}
