#include "Dominion/Dominion.h"

class Sandbox : public Dominion::Application
{
public:
	Sandbox()
	{
		WString s(L"ABC");
	}

	~Sandbox()
	{

	}
};

Dominion::Application* Dominion::CreateApplication()
{
	return new Sandbox();
}