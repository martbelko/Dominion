#include <Dominion.h>

class ExampleLayer : public Dominion::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		DM_INFO("ExampleLayer Update");
	}

	void OnEvent(Dominion::Event& e) override
	{
		DM_TRACE(e);
	}
};

class Sandbox : public Dominion::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Dominion::Application* Dominion::CreateApplication()
{
	return new Sandbox();
}