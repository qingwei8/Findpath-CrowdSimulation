#include "BasePathFinding.h"

using namespace hivePathFinding;

CBasePathFinding::CBasePathFinding(void)
{
}

CBasePathFinding::~CBasePathFinding(void)
{
}

//********************************************************************
//FUNCTION:
void hivePathFinding::CBasePathFinding::init(hiveSceneProcess::CBaseGraph* vGraph, const std::vector<boost::any>& vExtraParament)
{
	m_pGraph = vGraph;
	_extraInitV(vExtraParament);
}