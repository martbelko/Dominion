#pragma once

#include <string>

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Window.h"

namespace Dominion {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		static Application& Get();
	private:
		static Application* s_Application;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();

}
