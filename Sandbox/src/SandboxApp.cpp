#include <Dominion.h>
#include <Dominion/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Dominion::Application
{
public:
	Sandbox(Dominion::ApplicationCommandLineArgs args)
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{
	}
};

Dominion::Application* Dominion::CreateApplication(Dominion::ApplicationCommandLineArgs args)
{
	return new Sandbox(args);
}
