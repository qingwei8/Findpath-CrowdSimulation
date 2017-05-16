#include "VisionBasedPathFinding.h"
#include "AStarPathFinding.h"
#include "BaseScene.h"

using namespace hivePathFinding;

CVisionBasedPathFinding::CVisionBasedPathFinding(void)
{
}


CVisionBasedPathFinding::~CVisionBasedPathFinding(void)
{
}

//********************************************************************
//FUNCTION:
float hivePathFinding::CVisionBasedPathFinding::findPathV(const glm::vec2& vGridCenterStart, const glm::vec2& vGridCenterEnd, std::vector<glm::vec2>& vGridCenterRoadMap)
{
	std::vector<boost::any> ExtraInfo;
	CBasePathFinding* pAStarWithoutCrowd = new CAStarPathFinding;
	CBasePathFinding* pAStarWithCrowd    = new CAStarPathFinding;
	pAStarWithoutCrowd->init(m_pGraph, ExtraInfo);
	pAStarWithCrowd->init(m_pGraphWithCorwd, ExtraInfo);

	std::vector<const hiveSceneProcess::SRectShape*> GridInsightSet;
	m_pScene->dumpPassableSetInsightV(vGridCenterStart, m_VisionRadius, GridInsightSet);
	
	float ResultWeight = FLT_MAX;
	std::vector<glm::vec2> RoadInsight;
	std::vector<glm::vec2> RoadWithoutSight;

	for (auto Grid : GridInsightSet)
	{
		std::vector<glm::vec2> TempRoadInsight;
		std::vector<glm::vec2> TempRoadWithoutSight;
		int WeightInsight = pAStarWithCrowd->findPathV(vGridCenterStart, Grid->getCenter(), TempRoadInsight);
		int WeightWithoutSight = pAStarWithoutCrowd->findPathV(Grid->getCenter(),vGridCenterEnd, TempRoadWithoutSight);

		if ((WeightInsight + WeightWithoutSight) < ResultWeight)
		{
			RoadInsight = TempRoadInsight;
			RoadWithoutSight = TempRoadWithoutSight;
		}
	}
	vGridCenterRoadMap = RoadInsight;
	vGridCenterRoadMap.insert(vGridCenterRoadMap.end(), RoadWithoutSight.begin(), RoadWithoutSight.end());

	return ResultWeight;
}

//********************************************************************
//FUNCTION:
void hivePathFinding::CVisionBasedPathFinding::_extraInitV(const std::vector<boost::any>& vExtraParament)
{
	m_pGraphWithCorwd = boost::any_cast<hiveSceneProcess::CBaseGraph*>(vExtraParament[0]);
	m_pScene = boost::any_cast<hiveSceneProcess::CBaseScene*>(vExtraParament[1]);
	m_VisionRadius = boost::any_cast<int>(vExtraParament[2]);
}