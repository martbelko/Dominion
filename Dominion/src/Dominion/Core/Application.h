#pragma once

#include <string>

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Window.h"
#include "Dominion/Events/WindowEvent.h"

namespace Dominion {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		static Application& Get();
	private:
		bool OnWindowClosed(WindowClosedEvent& event);
	private:
		static Application* s_Application;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();

}
