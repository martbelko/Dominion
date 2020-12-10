#include "GraphLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GraphLayer::GraphLayer()
	: Dominion::Layer("Chess2D") {}

void GraphLayer::OnAttach()
{
	/* Setup camera */
	float wHeight = static_cast<float>(Dominion::Application::Get().GetWindow().GetHeight());
	float wWidth = static_cast<float>(Dominion::Application::Get().GetWindow().GetWidth());
	float ratio = wWidth / wHeight;

	m_Camera = Dominion::OrthographicCameraController(ratio, false);
	m_Camera.GetCamera().SetPosition(glm::vec3(3.5f, 3.5f, 0.5f));

	/* Some graph init */
	std::vector<Node> nodes;
	nodes.emplace_back(0);
	nodes.emplace_back(1);
	nodes.emplace_back(2);
	nodes.emplace_back(3);
	nodes.emplace_back(4);
	nodes.emplace_back(5);
	nodes.emplace_back(6);
	nodes.emplace_back(7);
	nodes.emplace_back(8);
	nodes.emplace_back(9);
	nodes.emplace_back(10);
	nodes.emplace_back(11);
	nodes.emplace_back(12);

	nodes[0].AddEdge(nodes[1], 1);
	nodes[0].AddEdge(nodes[9], 1);

	nodes[1].AddEdge(nodes[0], 1);
	nodes[1].AddEdge(nodes[8], 1);

	nodes[2].AddEdge(nodes[3], 1);

	nodes[3].AddEdge(nodes[7], 1);
	nodes[3].AddEdge(nodes[2], 1);
	nodes[3].AddEdge(nodes[4], 1);
	nodes[3].AddEdge(nodes[5], 1);

	nodes[4].AddEdge(nodes[3], 1);

	nodes[5].AddEdge(nodes[3], 1);
	nodes[5].AddEdge(nodes[6], 1);

	nodes[6].AddEdge(nodes[5], 1);
	nodes[6].AddEdge(nodes[7], 1);

	nodes[7].AddEdge(nodes[8], 1);
	nodes[7].AddEdge(nodes[10], 1);
	nodes[7].AddEdge(nodes[11], 1);
	nodes[7].AddEdge(nodes[6], 1);
	nodes[7].AddEdge(nodes[3], 1);

	nodes[8].AddEdge(nodes[1], 1);
	nodes[8].AddEdge(nodes[9], 1);
	nodes[8].AddEdge(nodes[7], 1);

	nodes[9].AddEdge(nodes[0], 1);
	nodes[9].AddEdge(nodes[8], 1);

	nodes[10].AddEdge(nodes[7], 1);
	nodes[10].AddEdge(nodes[11], 1);

	nodes[11].AddEdge(nodes[10], 1);
	nodes[11].AddEdge(nodes[7], 1);

	Graph g(nodes);
	g.FindComponents();
}

void GraphLayer::OnUpdate(const Dominion::Timestep& timestep)
{
	// Update
	m_Camera.OnUpdate(timestep);
	Dominion::Renderer2D::ResetStats();

	// Render
	Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Dominion::RenderCommand::ClearColorBuffer();
	Dominion::RenderCommand::ClearDepthBuffer();

	Dominion::Renderer2D::BeginScene(m_Camera.GetCamera());

	Dominion::Renderer2D::EndScene();
}

void GraphLayer::OnEvent(Dominion::Event& e)
{
	m_Camera.OnEvent(e);
}
