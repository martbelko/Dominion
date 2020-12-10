#pragma once

#include "Node.h"
#include "Edge.h"
#include "Graph.h"

#include <Dominion.h>

class GraphLayer : public Dominion::Layer
{
public:
	GraphLayer();
	virtual void OnAttach() override;
	virtual void OnUpdate(const Dominion::Timestep& timestep) override;
	virtual void OnEvent(Dominion::Event& e) override;
private:
	Dominion::OrthographicCameraController m_Camera;

	Node* m_StartNode;
};
