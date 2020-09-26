#include <Dominion.h>

#include <memory>

void Func(Dominion::MouseScrolledEvent& e)
{
	std::cout << "Works\n";
}

class Sandbox : public Dominion::Application
{
public:
	Sandbox()
	{
		Dominion::Event& x = Dominion::MouseMovedEvent(1254.1f, -5.4f);

	}

	~Sandbox()
	{

	}
};

Dominion::Application* Dominion::CreateApplication()
{
	return new Sandbox();
}