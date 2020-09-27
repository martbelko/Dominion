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
			m_Window = std::unique_ptr<Window>(Window::Create());
			m_Window->SetEventCallback(DM_BIND_EVENT_FN(Application::OnEvent));
		}
		else
		{
			DM_DEBUGBREAK();
		}
	}

	Application::~Application()
	{

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
	}

	Application& Dominion::Application::Get()
	{
		return *s_Application;
	}

}