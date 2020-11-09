#pragma once

#include <Dominion.h>

class Sandbox2D : public Dominion::Layer
{
public:
	Sandbox2D();
	void OnUpdate(const Dominion::Timestep& timestep) override;
	void OnEvent(Dominion::Event& e) override;
	void OnImGuiRender() override;
private:
	Dominion::Ref<Dominion::Texture2D> m_Texture2D;
	Dominion::Ref<Dominion::Texture2D> m_TestTexture;

	Dominion::OrthographicCamera m_Camera;

	float m_Rotation = 0.0f;
	float m_RotationSpeed = 40.0f;

	int m_Count = 20;
};
