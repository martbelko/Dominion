#include <Dominion.h>

#if defined(new)
#undef new
#include "../imgui/imgui.h"
#define new DEBUG_NEW
#else
#include "../imgui/imgui.h"
#endif

class ExampleLayer : public Dominion::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
		};

		unsigned int indices[] = {
			0, 1, 2
		};

		m_Pipeline = Dominion::Pipeline::Create(Dominion::VertexBuffer::Create(vertices, sizeof(vertices)), Dominion::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)), Dominion::InputLayout::Create(
			{
				{ "Position", Dominion::DataType::Float3 }
			}
		));

		m_Shader = Dominion::Shader::Create("Test", "TestVS.glsl", "TestPS.glsl");

		float vertices2[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		unsigned int indices2[] = {
			0, 1, 2
		};

		m_Pipeline2 = Dominion::Pipeline::Create(Dominion::VertexBuffer::Create(vertices2, sizeof(vertices2)), Dominion::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(unsigned int)), Dominion::InputLayout::Create(
			{
				{ "Position", Dominion::DataType::Float3 },
				{ "Color", Dominion::DataType::Float4 }
			}
		));

		m_Shader2 = Dominion::Shader::Create("Test2", "TestVS2.glsl", "TestPS2.glsl");
	}

	void OnUpdate(const Dominion::Timestep& timestep) override
	{
		Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Dominion::RenderCommand::Clear();

		Dominion::Renderer::BeginScene(m_Camera);
		Dominion::Renderer::Submit(m_Shader, m_Pipeline);
		Dominion::Renderer::Submit(m_Shader2, m_Pipeline2);
		Dominion::Renderer::EndScene();

		DM_TRACE("Timestep: {0}", timestep.GetFPS());
	}

	void OnEvent(Dominion::Event& e) override
	{

	}

	void OnImGuiRender() override
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}
private:
	Dominion::Ref<Dominion::Pipeline> m_Pipeline;
	Dominion::Ref<Dominion::Shader> m_Shader;

	Dominion::Ref<Dominion::Pipeline> m_Pipeline2;
	Dominion::Ref<Dominion::Shader> m_Shader2;

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
