#pragma once
#include <vector>
#include "BasePathFinding.h"

namespace hivePathFinding
{
	struct SPoint
	{
		SPoint() 
		{
			pParent             = NULL;
			CostOfStart2Current = 0.0;
			CostOfCurrent2End   = 0.0;
			TotalCost           = 0.0;
		}
		SPoint(const glm::vec2& vGrid, SPoint* vParent, float vCostOfStart2Current, float vCostOfCurrent2End)
		{
			Grid                = vGrid;
			pParent             = vParent;
			CostOfStart2Current = vCostOfStart2Current;
			CostOfCurrent2End   = vCostOfCurrent2End;
			TotalCost           = CostOfStart2Current + CostOfCurrent2End;
		}

		SPoint* pParent;
		glm::vec2 Grid;
		float CostOfStart2Current;
		float CostOfCurrent2End;
		float TotalCost;
	};

	class FIND_PATH_DLL_EXPORT CAStarPathFinding : public CBasePathFinding
	{
	public:
		CAStarPathFinding(void);
		~CAStarPathFinding(void);

		virtual float findPathV(const glm::vec2& vGridCenterStart, const glm::vec2& vGridCenterEnd, std::vector<glm::vec2>& vGridCenterRoadMap) override;

	private:
		SPoint* __fetchPointFromList(const glm::vec2& vGrid, std::vector<SPoint*>& vPointList) const;
		SPoint* __findOptimalPoint();
		bool __isInList(const std::vector<SPoint*>& vList, const glm::vec2& vSceneGrid) const;
		void __destructDataSet();
		void __updateOpenList(SPoint* vOptimalPoint, const glm::vec2& vGoalPosInScene);
		void __extractPath(std::vector<glm::vec2>& vRoadmap);
		float __exeFindPathProcess(const glm::vec2& vGridCenterStart, const glm::vec2& vGridCenterEnd);

		float __getCostOfSource2TargetPoint(const glm::vec2& vBeginNode, const glm::vec2& vEndNode) const;

	private:
		std::vector<SPoint*> m_OpenList;
		std::vector<SPoint*> m_ClosedList;
	};
}