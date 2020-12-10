#include "Graph.h"

#include <Dominion.h>

std::vector<std::vector<Node*>> Graph::FindComponents()
{
	bool* visited = new bool[m_Nodes.size()];
	memset(visited, false, m_Nodes.size() * sizeof(bool));

	std::vector<std::vector<Node*>> result = {};

	/*for (U32F i = 0; i < m_Nodes.size(); ++i)
	{
		if (!visited[i])
	}*/

	delete[] visited;
	return result;
}

void Graph::DFS(uint32_t startIndex)
{
	bool* visited = new bool[m_Nodes.size()];
	memset(visited, false, m_Nodes.size() * sizeof(bool));

	DFSRec(&m_Nodes[startIndex], visited);

	delete[] visited;
}

void Graph::DFSRec(Node* node, bool* visited)
{
	if (visited[node->GetValue()])
	{
		DM_TRACE("{0} was already visited, returning...", node->GetValue());
		return;
	}

	visited[node->GetValue()] = true;
	DM_TRACE("Visiting {0}", node->GetValue());

	std::vector<Edge> edges = node->GetEdges();
	for (Edge& edge : edges)
		DFSRec(edge.GetEndNode(), visited);
}
