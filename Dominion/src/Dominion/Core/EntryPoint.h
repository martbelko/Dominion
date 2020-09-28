#pragma once

extern Dominion::Application* Dominion::CreateApplication();

int main(int argc, char** argv)
{
	#if defined(DM_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	Dominion::Log::Init();
	Dominion::Log::Shutdown();
	DM_CORE_INFO("Initialized engine core logger");
	DM_INFO("Initialized client logger");

	Dominion::Application* app = Dominion::CreateApplication();
	app->Run();

	delete app;

	return 0;
}
