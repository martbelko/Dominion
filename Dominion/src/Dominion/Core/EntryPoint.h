#pragma once

extern Dominion::Application* Dominion::CreateApplication();

int main(int argc, char** argv)
{
	Dominion::Log::Init();
	DM_CORE_INFO("Initialized engine core logger");
	DM_INFO("Initialized client logger");

	Dominion::Application* app = Dominion::CreateApplication();
	app->Test();

	return 0;
}
