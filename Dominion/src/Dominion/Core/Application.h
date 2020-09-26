#pragma once

#include <string>

#include "Dominion/Core/Base.h"

namespace Dominion {

	class Application
	{
	public:
		Application(const std::string& name = "Dominion App");
		virtual ~Application();

		static Application& Get();
	private:
		static Application* s_Application;
		std::string m_Name;
	};

	Application* CreateApplication();

}
