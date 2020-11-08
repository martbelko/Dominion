#pragma once

#include "Dominion/Core/Base.h"

extern Dominion::Application* Dominion::CreateApplication();

int main(int argc, char** argv)
{
#if defined(DM_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	DM_PROFILE_BEGIN_SESSION("Startup", "DominionProfile-Startup.json");
	Dominion::Log::Init();
	DM_CORE_INFO("Initialized engine core logger");
	DM_INFO("Initialized client logger");

	Dominion::Application* app = Dominion::CreateApplication();
	DM_PROFILE_END_SESSION();

	DM_PROFILE_BEGIN_SESSION("Runtime", "DominionProfile-Runtime.json");
	app->Run();
	DM_PROFILE_END_SESSION();

	DM_PROFILE_BEGIN_SESSION("Shutdown", "DominionProfile-Shutdown.json");
	delete app;

	Dominion::Log::Shutdown();
	DM_PROFILE_END_SESSION();

	return 0;
}
