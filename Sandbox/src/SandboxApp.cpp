#include <Dominion.h>
#include <Dominion/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "Sandbox3D.h"
#include "Chess/Chess2D.h"
#include "Graphs/GraphLayer.h"

class Sandbox : public Dominion::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox3D());
	}

	~Sandbox()
	{

	}
};

Dominion::Application* Dominion::CreateApplication()
{
	return new Sandbox();
}
