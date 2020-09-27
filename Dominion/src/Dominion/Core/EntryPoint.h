#pragma once

#if defined(DM_DEBUG)
#define _CRTDBG_MAP_ALLOC // to get more details
#include <crtdbg.h>
#endif

extern Dominion::Application* Dominion::CreateApplication();

int main(int argc, char** argv)
{
#if defined(DM_DEBUG)
	_CrtMemState sOld;
	_CrtMemState sNew;
	_CrtMemState sDiff;
	_CrtMemCheckpoint(&sOld); // take a snapchot
#endif

	{
		Dominion::Log::Init();
		DM_CORE_INFO("Initialized engine core logger");
		DM_INFO("Initialized client logger");

		Dominion::Application* app = Dominion::CreateApplication();
		app->Run();
	}

#if defined(DM_DEBUG)
	_CrtMemCheckpoint(&sNew); // take a snapchot
	if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
	{
		OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
		_CrtMemDumpStatistics(&sDiff);
		OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
		_CrtMemDumpAllObjectsSince(&sOld);
		OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
		_CrtDumpMemoryLeaks();
	}
#endif

	return 0;
}
