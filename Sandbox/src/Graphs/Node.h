#pragma once

#include "Edge.h"

#include <vector>

class Node
{
public:
	Node(int value, std::vector<Edge> edges = std::vector<Edge>())
		: m_Value(value), m_Edges(edges) {}

	int GetValue() const { return m_Value; }

	const std::vector<Edge>& GetEdges() const { return m_Edges; }
	std::vector<Edge>& GetEdges() { return m_Edges; }

	void AddEdge(Node& endNode, int weight) { m_Edges.emplace_back(this, &endNode, weight); }
	void AddEdge(Node* endNode, int weight) { m_Edges.emplace_back(this, endNode, weight); }
private:
	int m_Value;
	std::vector<Edge> m_Edges;
};
