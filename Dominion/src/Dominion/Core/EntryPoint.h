#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Application.h"

#ifdef DM_PLATFORM_WINDOWS

	extern Dominion::Application* Dominion::CreateApplication(ApplicationCommandLineArgs args);

	int main(int argc, char** argv)
	{
		Dominion::Log::Init();

		DM_PROFILE_BEGIN_SESSION("Startup", "DominionProfile-Startup.json");
		auto app = Dominion::CreateApplication({ argc, argv });
		DM_PROFILE_END_SESSION();

		DM_PROFILE_BEGIN_SESSION("Runtime", "DominionProfile-Runtime.json");
		app->Run();
		DM_PROFILE_END_SESSION();

		DM_PROFILE_BEGIN_SESSION("Shutdown", "DominionProfile-Shutdown.json");
		delete app;
		DM_PROFILE_END_SESSION();
	}

#endif
