#pragma once

#include <Dominion.h>

class Sandbox3D : public Dominion::Layer
{
public:
	Sandbox3D();
	virtual void OnAttach() override;
	virtual void OnUpdate(const Dominion::Timestep& timestep) override;
	virtual void OnEvent(Dominion::Event& e) override;
private:
	bool OnKeyPressed(Dominion::KeyPressedEvent& e);
private:
	Dominion::Ref<Dominion::Model> m_Model;

	Dominion::PerspectiveCameraController m_Camera;

	bool m_ShowCursor = false;

};
