#include <Dominion.h>

#if defined(new)
#undef new
#include "../imgui/imgui.h"
#define new DEBUG_NEW
#else
#include "../imgui/imgui.h"
#endif

class ExampleLayer : public Dominion::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{

	}

	void OnEvent(Dominion::Event& e) override
	{

	}

	void OnImGuiRender() override
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
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
