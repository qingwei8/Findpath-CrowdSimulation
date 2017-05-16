#include "AStarPathFinding.h"
#include "BaseGraph.h"

using namespace hivePathFinding;

CAStarPathFinding::CAStarPathFinding(void)
{
}

CAStarPathFinding::~CAStarPathFinding(void)
{
	for (auto& Point : m_OpenList)
	{
		if (Point)
		{
			delete Point;
			Point = NULL;
		}
	}

	for (auto& Point : m_ClosedList)
	{
		if (Point)
		{
			delete Point;
			Point = NULL;
		}
	}
}

//********************************************************************
//FUNCTION:
float hivePathFinding::CAStarPathFinding::findPathV(const glm::vec2& vGridCenterStart, const glm::vec2& vGridCenterEnd, std::vector<glm::vec2>& vGridCenterRoadMap)
{
	float Weight = 0.0f;
	if ((Weight = __exeFindPathProcess(vGridCenterStart, vGridCenterEnd)) > 0.0f)
	{
		__extractPath(vGridCenterRoadMap);
		return Weight;
	}
	return 0.0f;
}

//********************************************************************
//FUNCTION:
float hivePathFinding::CAStarPathFinding::__exeFindPathProcess(const glm::vec2& vGridCenterStart, const glm::vec2& vGridCenterEnd)
{
	__destructDataSet();

	SPoint* pEndNode   = new SPoint(vGridCenterEnd, NULL, 0.0f, 0.0f);
	SPoint* pStartNode = new SPoint(vGridCenterStart, pEndNode, 0.0f, __getCostOfSource2TargetPoint(vGridCenterStart, vGridCenterEnd));
	m_OpenList.push_back(pStartNode);

	do 
	{
		SPoint* pOptimalPoint = __findOptimalPoint();
		if (!pOptimalPoint) 
		{
			delete pEndNode;
			return false;
		}		
		__updateOpenList(pOptimalPoint, vGridCenterEnd);
	}while (!(__isInList(m_OpenList, vGridCenterEnd)));

	pEndNode->pParent = m_ClosedList.back();
	pEndNode->TotalCost = pEndNode->pParent->TotalCost + m_pGraph->getEdgeWeight(pEndNode->pParent->Grid, vGridCenterEnd);
	m_ClosedList.push_back(pEndNode);
	return pEndNode->TotalCost;
}

//********************************************************************
//FUNCTION:
SPoint* hivePathFinding::CAStarPathFinding::__fetchPointFromList(const glm::vec2& vGrid, std::vector<SPoint*>& vPointList) const
{
	auto& Iter = vPointList.begin();
	for (; Iter!=vPointList.end(); ++Iter)
	{
		if ((*Iter)->Grid == vGrid)
			return *Iter;
	}

	return NULL;
}

//********************************************************************
//FUNCTION:
bool hivePathFinding::CAStarPathFinding::__isInList(const std::vector<SPoint*>& vList, const glm::vec2& vSceneGrid) const
{
	for (auto& Point : vList)
	{
		if (Point->Grid == vSceneGrid) return true;
	}
	return false;
}

//********************************************************************
//FUNCTION:
void hivePathFinding::CAStarPathFinding::__destructDataSet()
{
	for (auto& Point : m_OpenList)
		delete Point;

	for (auto& Point : m_ClosedList)
		delete Point;

	m_OpenList.resize(0);
	m_ClosedList.resize(0);
}

//********************************************************************
//FUNCTION:
SPoint* hivePathFinding::CAStarPathFinding::__findOptimalPoint()
{
	float MinCost            = FLT_MAX;
	float IsFindOptimalPoint = false;
	std::vector<SPoint*>::iterator OptimalPointIter;
	for (auto& Iter=m_OpenList.begin(); Iter!=m_OpenList.end(); ++Iter)
	{
		if ((*Iter)->TotalCost < MinCost)
		{
			MinCost            = (*Iter)->TotalCost;
			OptimalPointIter   = Iter;
			IsFindOptimalPoint = true;
		}
	}

	SPoint* pOptimalPoint = IsFindOptimalPoint ? *OptimalPointIter : NULL;
	if (pOptimalPoint)
	{
		m_ClosedList.push_back(pOptimalPoint);
		m_OpenList.erase(OptimalPointIter);
	}

	return pOptimalPoint;
}

//********************************************************************
//FUNCTION:
void hivePathFinding::CAStarPathFinding::__updateOpenList(SPoint* vOptimalPoint, const glm::vec2& vGoalPosInScene)
{
	std::vector<glm::vec2> AdjacentNodeSet;
	m_pGraph->getAdjacentNodeSet(vOptimalPoint->Grid, AdjacentNodeSet);

	for (auto& Node : AdjacentNodeSet)
	{
		if (__isInList(m_ClosedList, Node)) continue;
		else if(!__isInList(m_OpenList, Node))
		{
			float CostOfStart2Current = m_pGraph->getEdgeWeight(vOptimalPoint->Grid, Node) + vOptimalPoint->CostOfStart2Current;
			SPoint* pTempPoint        = new SPoint(Node, vOptimalPoint, CostOfStart2Current, __getCostOfSource2TargetPoint(Node, vGoalPosInScene));
			m_OpenList.push_back(pTempPoint);
		}
		else if (__isInList(m_OpenList, Node))
		{
			SPoint* pNowPoint = __fetchPointFromList(Node, m_OpenList);
			float GCost       = m_pGraph->getEdgeWeight(vOptimalPoint->Grid, Node) + vOptimalPoint->CostOfStart2Current;
			if (GCost < pNowPoint->CostOfStart2Current)
			{
				pNowPoint->pParent             = vOptimalPoint;
				pNowPoint->CostOfStart2Current = GCost;
				pNowPoint->TotalCost           = pNowPoint->CostOfCurrent2End + pNowPoint->CostOfStart2Current;
			}
		}
	}
}

//********************************************************************
//FUNCTION:
void hivePathFinding::CAStarPathFinding::__extractPath(std::vector<glm::vec2>& vRoadmap)
{
	vRoadmap.clear();
	SPoint* pEndPoint = m_ClosedList.back();
	do 
	{
		vRoadmap.push_back(pEndPoint->Grid);
		pEndPoint = pEndPoint->pParent;
	}while (!(pEndPoint->Grid == m_ClosedList.back()->Grid));
	std::reverse(vRoadmap.begin(), vRoadmap.end());

	vRoadmap.erase(vRoadmap.begin());
	vRoadmap.erase(vRoadmap.end() - 1);
}

//********************************************************************
//FUNCTION:
float hivePathFinding::CAStarPathFinding::__getCostOfSource2TargetPoint(const glm::vec2& vBeginNode, const glm::vec2& vEndNode) const
{
	return abs(vBeginNode.x - vEndNode.x) + abs(vBeginNode.y - vEndNode.y);
}