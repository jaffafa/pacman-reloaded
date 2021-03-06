#include "stdafx.h"
#include "Blinky.h"
#include "PathFinding.h"

Blinky::Blinky(Node** playerStartNode, Node** playerEndNode)
{
	m_playerStartNode = playerStartNode;
	m_playerEndNode	  = playerEndNode;
}

Blinky::~Blinky(void) { }

void Blinky::RoamingBehaviour(void)
{
	std::vector<Node*> availableNodes = GetAvailableNodes();

	m_start = m_end;

	UINT n = (UINT)availableNodes.size();
	if (n == 1)
	{
		m_end = availableNodes[0];

		if (!m_currentPath.empty())
			m_currentPath.pop_back();
	}
	else if (n > 1)
	{
		if (m_start == *m_playerStartNode)
			m_end = *m_playerEndNode;
		else
			m_end = Pathfinding::findPath(*m_playerStartNode, m_start).back();

		if (!IsEndNodeInVector(availableNodes))
			m_end = availableNodes[rand() % n];
	}
}