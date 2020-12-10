#pragma once

#include "Edge.h"
#include "Node.h"

#include <vector>

class Graph
{
public:
	Graph() = default;
	Graph(const std::vector<Node> nodes)
		: m_Nodes(nodes) {}

	std::vector<Node>& GetNodes() { return m_Nodes; }
	const std::vector<Node>& GetNodes() const { return m_Nodes; }

	void AddNode(const Node& node) { m_Nodes.push_back(node); }
	template<typename... Args>
	void AddNode(Args&&... args) { m_Nodes.emplace_back(std::forward<Args>(args)...); }

	std::vector<std::vector<Node*>> FindComponents();
private:
	void DFS(uint32_t startIndex = 0);
	void DFSRec(Node* node, bool* visited);
private:
	std::vector<Node> m_Nodes;
};
