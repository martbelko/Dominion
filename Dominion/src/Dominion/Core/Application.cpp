#include "dmpch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

namespace Dominion {

	Application* Application::s_Application = nullptr;

	Application::Application()
	{
		if (s_Application == nullptr)
		{
			s_Application = this;
			m_Window = Window::Create(DM_BIND_EVENT_FN(Application::OnEvent));
		}
		else
		{
			DM_DEBUGBREAK();
		}
	}

	Application::~Application()
	{
		delete m_Window;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		DM_CORE_INFO("{0}", event.ToString());
		event.Dispatch<WindowCreatedEvent>(DM_BIND_EVENT_FN(Application::OnWindowCreated));
		event.Dispatch<WindowClosedEvent>(DM_BIND_EVENT_FN(Application::OnWindowClosed));
	}

	bool Application::OnWindowCreated(WindowCreatedEvent& event)
	{
		DM_CORE_INFO(event);
		return true;
	}

	bool Application::OnWindowClosed(WindowClosedEvent& event)
	{
		DM_CORE_INFO(event);
		m_Window->Close();
		m_Running = false;
		return true;
	}

	Application& Dominion::Application::Get()
	{
		return *s_Application;
	}

}