#include "dmpch.h"
#include "Application.h"

#include "Dominion/Core/Input.h"

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
		}
		else
		{
			DM_DEBUGBREAK();
		}
	}

	Application::~Application()
	{
		m_LayerStack.PopOverlay(m_ImGuiLayer);
		delete m_ImGuiLayer;
		delete m_Window;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			static bool show = true;
			ImGui::ShowDemoWindow(&show);
			m_ImGuiLayer->End();

			//auto [x, y] = Input::GetMousePosition();
			//DM_CORE_TRACE("{0}; {1}", x, y);

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
