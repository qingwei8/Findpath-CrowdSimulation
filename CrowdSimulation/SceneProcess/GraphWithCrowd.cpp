#include "GraphWithCrowd.h"
#include "SceneGrid.h"

using namespace hiveSceneProcess;

CGraphWithCrowd::CGraphWithCrowd()
{
}

CGraphWithCrowd::~CGraphWithCrowd()
{
}

//********************************************************************
//FUNCTION:
float hiveSceneProcess::CGraphWithCrowd::_computeEdgeWeightV(const glm::vec2& vBegin, const glm::vec2& vEnd) const
{
	CSceneGrid* pBegin = m_pScene->getSceneGridV(vBegin);
	CSceneGrid* pEnd   = m_pScene->getSceneGridV(vEnd);

	glm::vec2 Begin2EndVec = glm::normalize(vEnd - vBegin);
	float Distance         = glm::distance(vBegin, vEnd);
	float Test = pEnd->getCrowdDensity();
	if (pEnd->getCrowdDensity() == 0.0f)
	{
		return Distance;
	}
	float CosAngle        = glm::dot(Begin2EndVec, pEnd->getAvgDirection());
	float ResistanceForce = CosAngle >= 0.0 ? 1.0 : ((1-CosAngle) * pEnd->getAvgSpeed());
	float Test3 = pEnd->getAvgSpeed();
	_ASSERT(ResistanceForce > 0.0);

	float Test2 = Distance * ResistanceForce * (1.0 + pEnd->getCrowdDensity());//Todo:need delete
	return Distance * ResistanceForce * (1.0 + pEnd->getCrowdDensity());
}