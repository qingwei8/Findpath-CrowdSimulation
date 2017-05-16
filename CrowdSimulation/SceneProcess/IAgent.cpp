#include "IAgent.h"

using namespace hiveSceneProcess;

CBaseAgent::CBaseAgent(const glm::vec2& vPosition, const glm::vec2& vDirection) : m_IsRVOAgentSpecified(false), m_IsReachedExit(false), m_IsNecessary2UpdateRoadMap(false), m_pRVOAgent(NULL), m_RVOIndex(-1)
{
	m_InitPos = vPosition;
	m_InitDir = vDirection;
	m_RoadMapSize = 0;
}

CBaseAgent::~CBaseAgent()
{
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseAgent::setRVOAgent(RVO::Agent* vAgent)
{
	_ASSERT(vAgent);
	m_pRVOAgent = vAgent;
	m_IsRVOAgentSpecified = true;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseAgent::setVelocity(const glm::vec2& vVelocity)
{
	_ASSERT(m_pRVOAgent);
	m_pRVOAgent->setVelocity(vVelocity);
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseAgent::setPreferVelocity(const glm::vec2& vPreferredVelocity)
{
	_ASSERT(m_pRVOAgent);
	m_pRVOAgent->setPrefVelocity(vPreferredVelocity);
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseAgent::setPosition(const glm::vec2& vPos)
{
	_ASSERT(m_pRVOAgent);
	m_pRVOAgent->setPosition(vPos);
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseAgent::setGoalPosition(const glm::vec2& vGoalPos)
{
	_ASSERT(m_pRVOAgent);
	m_pRVOAgent->setGoalPos(vGoalPos);
	setNecessary2UpdateRoadMap(true);
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseAgent::setMoveSpeed(float vSpeed)
{
	_ASSERT(m_pRVOAgent);
	m_pRVOAgent->setMoveSpeed(vSpeed);
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseAgent::setMaxSpeed(float vSpeed)
{
	_ASSERT(m_pRVOAgent);
	m_pRVOAgent->setMaxSpeed(vSpeed);
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseAgent::setGroupID(unsigned int vID)
{
	_ASSERT(m_pRVOAgent);
	m_pRVOAgent->setGroupID(vID);
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseAgent::setIgnorance()
{
	_ASSERT(m_IsRVOAgentSpecified);
	m_pRVOAgent->setIgnorance();
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CBaseAgent::isAware() const
{
	_ASSERT(m_IsRVOAgentSpecified);
	return m_pRVOAgent->isAwareness();
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CBaseAgent::isReachedTempGoal(float vReachTempGoalDist) const
{
	if (m_RoadMapSet.size() > 0)
	{
		if (glm::distance(getPosition(), m_RoadMapSet[0]) < vReachTempGoalDist)
		{
			return true;
		}
	}
	return false;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseAgent::eraseFirstRoadMapPoint()
{
	_ASSERT(m_RoadMapSet.size() > 0);
	m_RoadMapSet.erase(m_RoadMapSet.begin());
	m_RoadMapSize = m_RoadMapSet.size();
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CBaseAgent::isReachedGoal(float vDistanceRange)
{
	float Test = glm::distance(getPosition(), getGoalPosition());
	return glm::distance(getPosition(), getGoalPosition()) < vDistanceRange;
}