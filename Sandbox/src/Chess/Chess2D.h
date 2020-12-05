#pragma once

#include <Dominion.h>

#include "Checkerboard.h"

class Chess2DLayer : public Dominion::Layer
{
public:
	Chess2DLayer();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const Dominion::Timestep& timestep) override;
	virtual void OnEvent(Dominion::Event& e) override;
private:
	bool OnKeyPressed(Dominion::KeyPressedEvent& e);
	bool OnMousePressed(Dominion::MousePressedEvent& e);
	bool OnMouseMoved(Dominion::MouseMovedEvent& e);
private:
	Dominion::OrthographicCameraController m_Camera;

	Checkerboard* m_Checkerboard;

	const Square* m_HoveredSquare = nullptr;
	const Chessman* m_SelectedChessman = nullptr;
	std::vector<Square*> m_PossibleSquares = {};

	static constexpr glm::vec4 m_WhiteColor = glm::vec4(0.949f, 0.8784f, 0.6588f, 1.0f);
	static constexpr glm::vec4 m_BlackColor = glm::vec4(0.6039f, 0.1725f, 0.0078f, 1.0f);
	static constexpr glm::vec4 m_SelectedSquareColor = glm::vec4(0.0627f, 0.4118f, 0.0235f, 1.0f);
	static constexpr glm::vec4 m_HoveredSquareColor = glm::vec4(1.0f, 0.996f, 0.0f, 1.0f);
	static constexpr glm::vec4 m_PossibleMoveColor = glm::vec4(0.0039f, 0.3333f, 0.6863f, 1.0f);
};
