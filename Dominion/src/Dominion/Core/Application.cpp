#include "dmpch.h"
#include "Application.h"

#include "Dominion/Core/Input.h"
#include "Dominion/Renderer/RenderCommand.h"
#include "Dominion/Renderer/Buffer.h"
#include "Dominion/Renderer/InputLayout.h"

#if defined(new)
	#undef new
	#include <imgui.h>
	#define new DEBUG_NEW
#else
	#include <imgui.h>
#endif

#include <Glad/glad.h> // Temporary

namespace Dominion {

	Application* Application::s_Application = nullptr;

	Application::Application()
	{
		if (s_Application == nullptr)
		{
			s_Application = this;
			m_Window = Window::Create(DM_BIND_EVENT_FN(Application::OnEvent));

			m_ImGuiLayer = new ImGuiLayer();
			PushOverlay(m_ImGuiLayer);

			RenderCommand::Init();

			float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			};

			m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

			m_Layout = InputLayout::Create(
				{
					{ "Position", DataType::Float3 }
				}
			);

			unsigned int indices[] = {
				0, 1, 2
			};

			m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));

			m_Shader = Shader::Create("Test", "TestVS.glsl", "TestPS.glsl");

			float vertices2[] = {
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
			};

			m_VertexBuffer2 = VertexBuffer::Create(vertices2, sizeof(vertices2));

			m_Layout2 = InputLayout::Create(
				{
					{ "Position", DataType::Float3 },
					{ "Color", DataType::Float4 }
				}
			);

			unsigned int indices2[] = {
				0, 1, 2
			};

			m_IndexBuffer2 = IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(unsigned int));

			m_Shader2 = Shader::Create("Test2", "TestVS2.glsl", "TestPS2.glsl");
		}
		else
		{
			DM_DEBUGBREAK();
		}
	}

	Application::~Application()
	{
		delete m_Shader;
		delete m_Shader2;
		m_LayerStack.PopOverlay(m_ImGuiLayer);
		delete m_ImGuiLayer;
		delete m_Window;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			RenderCommand::Clear();

			m_Shader->Bind();
			m_Layout->Bind();
			m_VertexBuffer->Bind();
			m_IndexBuffer->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_Shader2->Bind();
			m_Layout2->Bind();
			m_VertexBuffer2->Bind();
			m_IndexBuffer2->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer2->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		e.Dispatch<WindowCreatedEvent>(DM_BIND_EVENT_FN(Application::OnWindowCreated));
		e.Dispatch<WindowClosedEvent>(DM_BIND_EVENT_FN(Application::OnWindowClosed));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	Dominion::Window& Application::GetWindow() const
	{
		return *m_Window;
	}

	bool Application::OnWindowCreated(WindowCreatedEvent& event)
	{
		return false;
	}

	bool Application::OnWindowClosed(WindowClosedEvent& event)
	{
		m_Window->Close();
		m_Running = false;
		return false;
	}

	Application& Dominion::Application::Get()
	{
		return *s_Application;
	}

}
