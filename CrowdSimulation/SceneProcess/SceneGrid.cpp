#include "SceneGrid.h"

using namespace hiveSceneProcess;

CSceneGrid::CSceneGrid(const SRectShape* pRect, bool vIsPassable) : m_VelocityUpdated(false), m_SpeedUpdated(false), m_AvgSpeed(0.0)
{
	_ASSERT(pRect);
	m_pRect = pRect;
	m_IsPassable = vIsPassable;
}

CSceneGrid::~CSceneGrid()
{
	delete m_pRect;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CSceneGrid::addAgent(CBaseAgent* vAgent)
{
	_ASSERT(vAgent);

	m_AgentSet.push_back(vAgent);
	m_VelocityUpdated = false;
	m_SpeedUpdated    = false;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CSceneGrid::removeAgent(CBaseAgent* vAgent)
{
// 	for (unsigned int i=0; i<m_AgentSet.size(); ++i)
// 	{
// 		if ()
// 		{
// 		}
// 	}

	m_VelocityUpdated = false;
	m_SpeedUpdated    = false;
}

//********************************************************************
//FUNCTION:
float hiveSceneProcess::CSceneGrid::getCrowdDensity() const
{
	int LengthOfX = m_pRect->m_RightDown.x - m_pRect->m_LeftUp.x;
	int LengthOfY = m_pRect->m_RightDown.y - m_pRect->m_LeftUp.y;
	return m_AgentSet.size() / (float)(LengthOfX * LengthOfY);
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveSceneProcess::CSceneGrid::getAvgDirection()
{
	if (!m_VelocityUpdated) __updateCrowdAvgVelocity();
	return m_AvgDirection;
}

float hiveSceneProcess::CSceneGrid::getAvgSpeed()
{
	if (!m_SpeedUpdated) __updateCrowdAvgSpeed();
	return m_AvgSpeed;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CSceneGrid::__updateCrowdAvgVelocity()
{
	m_AvgDirection = glm::vec2(0, 0);

	if (m_AgentSet.size() <= 0)
	{
		return;
	}
	for (auto& Agent : m_AgentSet)
	{
		m_AvgDirection += Agent->getVelocity();
	}
	m_AvgDirection /= m_AgentSet.size();	
	m_VelocityUpdated = true;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CSceneGrid::__updateCrowdAvgSpeed()
{
	m_AvgSpeed = 0.0;

	if (m_AgentSet.size() <= 0)
	{
		return;
	}

	for (auto& Agent : m_AgentSet)
	{
		m_AvgSpeed += Agent->getMoveSpeed();
	}

	m_AvgSpeed /= m_AgentSet.size();
	m_SpeedUpdated = true;
}