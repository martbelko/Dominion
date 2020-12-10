#pragma once

class Node;

class Edge
{
public:
	Edge(Node* startNode, Node* endNode, int weight)
		: m_Start(startNode), m_End(endNode), m_Weight(weight) {}

	Node* GetStartNode() const { return m_Start; }
	Node* GetEndNode() const { return m_End; }

	int GetWeight() const { return m_Weight; }
private:
	Node* m_Start, *m_End;
	int m_Weight;
};
