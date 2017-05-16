#include "BaseGraph.h"
#include <iostream>

using namespace hiveSceneProcess;

CBaseGraph::CBaseGraph() : m_pScene(NULL)
{
}

CBaseGraph::~CBaseGraph()
{
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseGraph::genGraph(const CBaseScene* vScene)
{
	_ASSERT(vScene);
	m_pScene = vScene;

	std::vector<const SRectShape*> PassableSet;
	m_pScene->dumpPassableSet(PassableSet);

	for (unsigned int i=0; i<PassableSet.size(); ++i)
	{
		__addNode(PassableSet[i]->getCenter());
	}

	for (unsigned int i=0; i<m_Node2IndexVec.size(); ++i)
	{
		__findAndAddEdgesForAGrid(m_Node2IndexVec[i].second);
	}
}

//********************************************************************
//FUNCTION:
float hiveSceneProcess::CBaseGraph::getEdgeWeight(const glm::vec2& vBeginNode, const glm::vec2& vEndNode) const
{
	return m_Graph.getEdgeWeight(__node2Index(vBeginNode), __node2Index(vEndNode));
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseGraph::getAdjacentNodeSet(const glm::vec2& vTargetNode, std::vector<glm::vec2>& voAdjacentNodeSet) const
{
	std::vector<GRAPHVERTEX> AdjacentNodeSet;
	GRAPHVERTEX Index = __node2Index(vTargetNode);
	if (Index<0) return;
	m_Graph.dumpAdjacentVertexSet(Index, AdjacentNodeSet);

	for (unsigned int i=0; i<AdjacentNodeSet.size(); i++)
		voAdjacentNodeSet.push_back(__index2Node(AdjacentNodeSet[i]));
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseGraph::__addNode(glm::vec2 vGrid)
{
	GRAPHVERTEX Index = m_Graph.addVertex(vGrid);
	m_Node2IndexVec.push_back(std::make_pair(Index, vGrid));
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseGraph::__addEdge(const glm::vec2& vBeginNode, const glm::vec2& vEndNode, float vWeight)
{
	_ASSERT(vWeight > 0.0);

	GRAPHVERTEX BeginNodeIndex = __node2Index(vBeginNode);
	GRAPHVERTEX EndNodeIndex   = __node2Index(vEndNode);
	m_Graph.addEdge(BeginNodeIndex, EndNodeIndex, vWeight);
}

//********************************************************************
//FUNCTION:
hiveSceneProcess::GRAPHVERTEX hiveSceneProcess::CBaseGraph::__node2Index(const glm::vec2& vTargetNode) const
{
	for (unsigned int i=0; i<m_Node2IndexVec.size(); ++i)
	{
		if (vTargetNode == (m_Node2IndexVec[i].second))
			return m_Node2IndexVec[i].first;
	}
	return -1;
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveSceneProcess::CBaseGraph::__index2Node(const GRAPHVERTEX& vTargetIndex) const
{
	for (unsigned int i=0; i<m_Node2IndexVec.size(); ++i)
	{
		if (vTargetIndex == m_Node2IndexVec[i].first)
			return m_Node2IndexVec[i].second;
	}

	return glm::vec2(0, 0);
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseGraph::__findAndAddEdgesForAGrid(glm::vec2& vNode)
{
	_ASSERT(m_pScene);

	std::vector<const SRectShape*> AdjNodeSet;
	m_pScene->dumpAdjacentPassableSetV(vNode, AdjNodeSet);

	for (unsigned int i=0; i<AdjNodeSet.size(); ++i)
	{
		//std::cout << AdjNodeSet[i]->getCenter().x << " " << AdjNodeSet[i]->getCenter().y << std::endl;
		__addEdge(vNode, AdjNodeSet[i]->getCenter(), _computeEdgeWeightV(vNode, AdjNodeSet[i]->getCenter()));
		__addEdge(AdjNodeSet[i]->getCenter(), vNode, _computeEdgeWeightV(AdjNodeSet[i]->getCenter(), vNode));
	}
}