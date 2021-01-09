#pragma once

#include <Dominion.h>

class Sandbox2D : public Dominion::Layer
{
public:
	Sandbox2D();
	virtual void OnAttach() override;
	virtual void OnUpdate(const Dominion::Timestep& timestep) override;
	virtual void OnEvent(Dominion::Event& e) override;
private:
	bool OnKeyPressed(Dominion::KeyPressedEvent& e);
private:
	Dominion::Scene m_Scene{ "Default Scene" };
	Dominion::Entity m_Square;
	Dominion::Entity m_Camera;
	Dominion::Entity m_Plane;
};
