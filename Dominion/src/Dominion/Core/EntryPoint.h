#pragma once

#include "Dominion/Core/Base.h"

extern Dominion::Application* Dominion::CreateApplication();

int main(int argc, char** argv)
{
#if defined(DM_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
#endif

	DM_PROFILE_BEGIN_SESSION("Startup", "DominionProfile-Startup.json");
	Dominion::Filesystem::Init();
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

	Dominion::Physics::Shutdown();
	Dominion::Log::Shutdown();
	DM_PROFILE_END_SESSION();

	return 0;
}
