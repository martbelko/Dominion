#include "dmpch.h"
#include "Application.h"

#include "Dominion/Core/Input.h"
#include "Dominion/Renderer/RenderCommand.h"

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

			glCreateVertexArrays(1, &m_VertexArray);
			glBindVertexArray(m_VertexArray);

			glCreateBuffers(1, &m_VertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

			float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

			glCreateBuffers(1, &m_IndexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

			unsigned int indices[] = {
				0, 1, 2
			};
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			m_Shader = Shader::Create("Test", "TestVS.glsl", "TestPS.glsl");
		}
		else
		{
			DM_DEBUGBREAK();
		}
	}

	Application::~Application()
	{
		delete m_Shader;
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

			glBindVertexArray(m_VertexArray);
			m_Shader->Bind();
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
