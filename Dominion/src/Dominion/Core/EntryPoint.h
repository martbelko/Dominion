#pragma once

#include "Dominion/Core/Application.h"

extern Dominion::Application* Dominion::CreateApplication();

int main(int argc, char** argv)
{
	Dominion::Application* app = Dominion::CreateApplication();
	app->Test();

	return 0;
}
