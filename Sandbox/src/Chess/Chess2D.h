#pragma once

#include <Dominion.h>

#include "Game.h"

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

	Checkerboard m_Checkerboard;
};
