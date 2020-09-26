#pragma once

extern Dominion::Application* Dominion::CreateApplication();

int main(int argc, char** argv)
{
	Dominion::Log::Init();

	Dominion::Application* app = Dominion::CreateApplication();
	app->Test();

	return 0;
}
