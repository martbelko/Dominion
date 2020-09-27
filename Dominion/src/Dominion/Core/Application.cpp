#include "dmpch.h"
#include "Application.h"

namespace Dominion {

	Application* Application::s_Application = nullptr;

	Application::Application()
	{
		if (s_Application == nullptr)
		{
			s_Application = this;
			m_Window = std::unique_ptr<Window>(Window::Create());
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
			m_Window->OnUpdate();
		}
	}

	Application& Dominion::Application::Get()
	{
		return *s_Application;
	}

}