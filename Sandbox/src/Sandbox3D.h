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
	Dominion::PerspectiveCameraController m_Camera;

	Dominion::Ref<Dominion::Shader> ourShader;
	Dominion::Ref<Dominion::Model> ourModel;
};
