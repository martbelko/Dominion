#include "Dominion/Core/EntryPoint.h"
#include "Dominion/Core/Application.h"

class Sandbox : public Dominion::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Dominion::Application* Dominion::CreateApplication()
{
	return new Sandbox();
}