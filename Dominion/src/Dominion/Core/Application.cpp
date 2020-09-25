#include "dmpch.h"
#include "Application.h"

namespace Dominion {

	Application* Application::s_Application = nullptr;

	Application::Application(const std::string& name /*= "Dominion App"*/)
	{
		if (s_Application == nullptr)
		{
			s_Application = this;
		}
		else
		{
			DM_DEBUGBREAK();
		}
	}

	Application::~Application()
	{

	}

	Application& Dominion::Application::Get()
	{
		return *s_Application;
	}

	void Application::Test()
	{
		std::cout << "Application test" << '\n';
		system("pause");
	}

}