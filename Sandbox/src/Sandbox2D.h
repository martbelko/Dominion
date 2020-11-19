#pragma once

#include <Dominion.h>

class Sandbox2D : public Dominion::Layer
{
public:
	Sandbox2D();
	virtual void OnAttach() override;
	virtual void OnUpdate(const Dominion::Timestep& timestep) override;
	virtual void OnEvent(Dominion::Event& e) override;
	virtual void OnImGuiRender() override;
private:
	bool OnKeyPressed(Dominion::KeyPressedEvent& e);
private:
	Dominion::Ref<Dominion::Texture2D> m_Texture2D;
	Dominion::Ref<Dominion::Texture2D> m_TestTexture;

	Dominion::PerspectiveCameraController m_Camera;

	bool m_ShowCursor = false;

	float m_Rotation = 0.0f;
	float m_RotationSpeed = 40.0f;

	int m_Count = 10;
};
