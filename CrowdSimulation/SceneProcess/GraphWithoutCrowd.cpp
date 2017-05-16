#include "GraphWithoutCrowd.h"

using namespace hiveSceneProcess;

CGraphWithoutCrowd::CGraphWithoutCrowd()
{
}

CGraphWithoutCrowd::~CGraphWithoutCrowd()
{
}

//********************************************************************
//FUNCTION:
float hiveSceneProcess::CGraphWithoutCrowd::_computeEdgeWeightV(const glm::vec2& vBegin, const glm::vec2& vEnd) const 
{
	return glm::distance(vBegin, vEnd);
}