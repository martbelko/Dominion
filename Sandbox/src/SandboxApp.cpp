#include <Dominion.h>
#include <Dominion/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Dominion::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Dominion::Application* Dominion::CreateApplication()
{
	return new Sandbox();
}
