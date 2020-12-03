#pragma once

#include <Dominion.h>

class Chess2DLayer : public Dominion::Layer
{
public:
	Chess2DLayer();
	virtual void OnAttach() override;
	virtual void OnUpdate(const Dominion::Timestep& timestep) override;
	virtual void OnEvent(Dominion::Event& e) override;
private:
	bool OnKeyPressed(Dominion::KeyPressedEvent& e);
private:
	Dominion::OrthographicCameraController m_Camera;

	const glm::vec4 colorWhite = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	const glm::vec4 colorBlack = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
};
