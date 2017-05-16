#include "CrowdSimulation.h"
#include <algorithm>
#include <crtdbg.h>
#include "SimulationCommon.h"
#include "CrowdSimulationConfig.h"
#include "AgentConfig.h"
#include "SceneGrid.h"

using namespace hiveCrowdSimulation;

CCrowdSimulation::CCrowdSimulation() : m_pScene(NULL), m_pGraphWithCrowd(NULL), m_pGraphWithoutCrowd(NULL), m_pPathFindMethod(NULL), m_NumElapsedFrame(0)
{
	m_pRVOSimulator   = new RVO::RVOSimulator();
	m_pNeighborSearch = new CKNNSearch();
	m_NumNoGroupAgent = 0;
}

CCrowdSimulation::~CCrowdSimulation()
{
	delete m_pNeighborSearch;
	delete m_pRVOSimulator;
	
	if (m_pScene)
	{
		delete m_pScene;
		m_pScene = NULL;
	}
	
	if (m_pGraphWithCrowd)
	{
		delete m_pGraphWithCrowd;
		m_pGraphWithCrowd = NULL;
	}
	
	if (m_pGraphWithoutCrowd)
	{
		delete m_pGraphWithoutCrowd;
		m_pGraphWithoutCrowd = NULL;
	}
	
	if (m_pPathFindMethod)
	{
		delete m_pPathFindMethod;
		m_pPathFindMethod = NULL;
	}

	for (auto& Agent : m_AgentSet)
	{
		if (Agent)
		{
			delete Agent;
			Agent = NULL;
		}
	}
	for (auto& Group : m_GroupSet)
	{
		if (Group)
		{
			delete Group;
			Group = NULL;
		}
	}
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CCrowdSimulation::isFinished()
{
	bool IsSimulationFinished = true;
	float RadiusOfGoal = CCrowdSimulationConfig::getInstance()->getAttribute<float>(SIMULATION_CONFIG_KEYWORD::RADIUS_OF_GOAL);
	for (auto& Agent : m_AgentSet)
	{
		if (!Agent->updateFinishStateV(RadiusOfGoal))
		{
			IsSimulationFinished = false;
		}
	}

	for (auto& Group : m_GroupSet)
	{
		if (!Group->updateFinishState(RadiusOfGoal))
		{
			IsSimulationFinished = false;
		}
	}

	return IsSimulationFinished;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::advance()
{
	//std::cout << "advance() " << std::endl;
	if (CCrowdSimulationConfig::getInstance()->getAttribute<float>(SIMULATION_CONFIG_KEYWORD::AWARENESS_PERCENTAGE) < 1.0)
	{
		__updateNeighbors();
	}
	__decideGoalDirectionAndUpdateRoadMap();
  	//__decideGoalPosition();
   // __updateRoadMap();
	//std::cout << "__decideGoalDirectionAndUpdateRoadMap()" << std::endl;
	__updatePreferVelocity();
	//std::cout << "__updatePreferVelocity();" << std::endl;
	m_pRVOSimulator->doStep();
	//std::cout << "m_pRVOSimulator->doStep();" << std::endl;
	//update Scene if necessary
	m_NumElapsedFrame++;
	__updateNessarity2FindNewGoal(m_NumElapsedFrame);

	std::cout << " 帧数： " << m_NumElapsedFrame << std::endl;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::initSimulator()
{	
	float NeighborDist    = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::NEIGHBOR_DIST);
	int NumMaxNeighbors   = g_MaxNeighbors;
	float TimeHorizonObst = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::TIME_HORIZON_OBST);
	float TimeHorizon     = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::TIME_HORIZON);
	float AgentRadius     = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::RADIUS);
	float MaxSpeed        = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::MAX_SPEED);

	m_pRVOSimulator->setTimeStep(CCrowdSimulationConfig::getInstance()->getAttribute<float>(SIMULATION_CONFIG_KEYWORD::TIME_STEP));
	m_pRVOSimulator->setAgentDefaults(NeighborDist, NumMaxNeighbors, TimeHorizon, TimeHorizonObst, AgentRadius, MaxSpeed);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::addObstacleToRVO()
{
	_ASSERT(m_pScene);

 	__addObstacleToRVO();
 	m_pRVOSimulator->processObstacles();
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::__addObstacleToRVO()
{
	std::vector<const hiveSceneProcess::SRectShape*> ObstacleSet;
	m_pScene->dumpObstacleSet(ObstacleSet);

	float RelaxRadius = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::RADIUS);
	RelaxRadius /= 2.0;

	for (unsigned int i=0; i<ObstacleSet.size(); ++i)
	{
		glm::vec2 LeftUp    = ObstacleSet[i]->m_LeftUp;
		glm::vec2 RightDown =  ObstacleSet[i]->m_RightDown;
	    glm::vec2 LeftDown  = glm::vec2(LeftUp.x, RightDown.y);
		glm::vec2 RightUp   = glm::vec2(RightDown.x, LeftUp.y);

		RightUp.x   += RelaxRadius; RightUp.y   -= RelaxRadius;
		LeftUp.x    -= RelaxRadius; LeftUp.y    -= RelaxRadius;
		LeftDown.x  -= RelaxRadius; LeftDown.y  += RelaxRadius;
		RightDown.x += RelaxRadius; RightDown.y += RelaxRadius;

		std::vector<glm::vec2> TempObstacle;
		TempObstacle.push_back(RightUp);
		TempObstacle.push_back(LeftUp);
		TempObstacle.push_back(LeftDown);
		TempObstacle.push_back(RightDown);
	    m_pRVOSimulator->addObstacle(TempObstacle);
	}

	//Todo: 添加边界
	int Width = m_pScene->getSceneWidthInPixel();
	int Height = m_pScene->getSceneHeightInPixel();
	std::vector<glm::vec2> BorderObstacle;

	BorderObstacle.push_back(glm::vec2(Width - RelaxRadius, 0));
	BorderObstacle.push_back(glm::vec2(RelaxRadius,         0));
	BorderObstacle.push_back(glm::vec2(RelaxRadius,         RelaxRadius));
	BorderObstacle.push_back(glm::vec2(Width - RelaxRadius, RelaxRadius));
	m_pRVOSimulator->addObstacle(BorderObstacle);

	BorderObstacle.clear();
	BorderObstacle.push_back(glm::vec2(RelaxRadius, 0));
	BorderObstacle.push_back(glm::vec2(0,           0));
	BorderObstacle.push_back(glm::vec2(0,           Height));
	BorderObstacle.push_back(glm::vec2(RelaxRadius, Height));
	m_pRVOSimulator->addObstacle(BorderObstacle);

	BorderObstacle.clear();
	BorderObstacle.push_back(glm::vec2(Width - RelaxRadius, Height - RelaxRadius));
	BorderObstacle.push_back(glm::vec2(RelaxRadius,         Height - RelaxRadius));
	BorderObstacle.push_back(glm::vec2(RelaxRadius,         Height));
	BorderObstacle.push_back(glm::vec2(Width - RelaxRadius, Height));
	m_pRVOSimulator->addObstacle(BorderObstacle);

	BorderObstacle.clear();
	BorderObstacle.push_back(glm::vec2(Width,               RelaxRadius));
	BorderObstacle.push_back(glm::vec2(Width - RelaxRadius, RelaxRadius));
	BorderObstacle.push_back(glm::vec2(Width - RelaxRadius, Height - RelaxRadius));
	BorderObstacle.push_back(glm::vec2(Width,               Height - RelaxRadius));
	m_pRVOSimulator->addObstacle(BorderObstacle);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::__updateNeighbors()
{
	std::vector<glm::vec2> CrowdPosSet;
	for (unsigned int i=0; i<m_pRVOSimulator->getNumAgents(); ++i)
	{
		CrowdPosSet.push_back(m_pRVOSimulator->getAgent(i)->getPosition());
	}
	
	_ASSERT(g_MaxNeighbors <= m_pRVOSimulator->getNumAgents() - 1);
	m_pNeighborSearch->initKNNSearch(CrowdPosSet, g_Dimension, g_MaxNeighbors);

	for (unsigned int i=0; i<m_AgentSet.size(); ++i)
	{
		if (m_AgentSet[i]->isAware())
		{
			continue;
		}

		std::vector<glm::vec2> NeighborPosSet, NeighborVelSet;
		std::vector<bool> NeighborAwareSet;
		std::vector<float> MoveSpeedSet;
		__findNeighborsInfo(m_AgentSet[i]->getPosition(), NeighborPosSet, NeighborVelSet, NeighborAwareSet, MoveSpeedSet);
		m_AgentSet[i]->setNeighborInfo(NeighborPosSet, NeighborVelSet, NeighborAwareSet, MoveSpeedSet);
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::__findNeighborsInfo(const glm::vec2& vPos, std::vector<glm::vec2>& voNeighborPosSet, std::vector<glm::vec2>& voNeighborVelSet, std::vector<bool>& voNeighborAwareSet, std::vector<float>& voSpeedSet)
{
	std::vector<unsigned int> NeighborIndexSet;
	m_pNeighborSearch->executeKNN(vPos, NeighborIndexSet);

	for (unsigned int i=0; i<NeighborIndexSet.size(); ++i)
	{
		RVO::Agent* pNeighbor = m_pRVOSimulator->getAgent(NeighborIndexSet[i]);
		if (glm::distance(vPos, pNeighbor->getPosition()) > CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::NEIGHBOR_DIST))
		{
			continue;
		}
		if (!m_pScene->isDirectVisibleV(vPos, pNeighbor->getPosition()))
		{
			continue;
		}
		voNeighborPosSet.push_back(pNeighbor->getPosition());
		voNeighborVelSet.push_back(pNeighbor->getVelocity());
		voNeighborAwareSet.push_back(pNeighbor->isAwareness());
		voSpeedSet.push_back(pNeighbor->getMoveSpeed());
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::__decideGoalDirectionAndUpdateRoadMap()
{
	for (unsigned int i=0; i<m_AgentSet.size(); ++i)
	{
		//std::cout << "第几个人： " << i << " " << m_AgentSet[i]->isAware() << std::endl;
		if (m_NumElapsedFrame && m_AgentSet[i]->isAware() && !m_AgentSet[i]->isNecessary2FindNewGoal())
		{
			continue;
		}

		m_AgentSet[i]->decideGoalPosition();
		if (!m_AgentSet[i]->isNeed2ComputeRoadMap())
		{
			continue;
		}

		std::vector<glm::vec2> FindPathResult;
		__computeGlobalRoadMap(m_AgentSet[i]->getPosition(), m_AgentSet[i]->getGoalPosition(), FindPathResult);
		FindPathResult.push_back(m_AgentSet[i]->getGoalPosition());
		m_AgentSet[i]->setRoadMap(FindPathResult);
		m_AgentSet[i]->setNeedComputeRoadMap(false);
	}

	for (unsigned int i=0; i<m_GroupSet.size(); ++i)
	{
		CGroup* pOneGroup = m_GroupSet[i];
		_ASSERT(pOneGroup->getNumGroupMembers() > 0);

		if (!m_NumElapsedFrame)
		{
			pOneGroup->decideGoalPosition();
		}

		if (m_NumElapsedFrame && pOneGroup->isNecessary2FindNewGoal())
		{
			pOneGroup->decideGoalPosition();
		}

		for (unsigned int k=0; k<pOneGroup->getNumGroupMembers(); ++k)
		{
			CDerivedAgent* pOneGroupMember = pOneGroup->getGroupMember(k);
			//std::cout << "组员 " << k << " : " << pOneGroupMember->getGoalPosition().x << " " << pOneGroupMember->getGoalPosition().y << std::endl;
			if (!pOneGroupMember->isRoadMapEmpty())
			{
				continue;
			}

			std::vector<glm::vec2> GlobalRoadMap;
			__computeGlobalRoadMap(pOneGroupMember->getPosition(), pOneGroup->getGroupTempGoal(), GlobalRoadMap);
			if (pOneGroupMember->isAware())
			{
				pOneGroupMember->setGoalPosition(pOneGroup->getGroupTempGoal());
				GlobalRoadMap.push_back(pOneGroup->getGroupTempGoal());
				pOneGroupMember->setRoadMap(GlobalRoadMap);
			}
			else
			{
				std::vector<glm::vec2> VisibleRoadMap;
				__chooseVisibleRoadMap(pOneGroupMember->getPosition(), GlobalRoadMap, CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::VISIBLE_RANGE), VisibleRoadMap);
				pOneGroupMember->setRoadMap(VisibleRoadMap);
			}
		}
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::__computeGlobalRoadMap(const glm::vec2& vStartPos, const glm::vec2& vEndPos, std::vector<glm::vec2>& voResultSet)
{
	glm::vec2 StartGridCenterPos = m_pScene->getSceneGridCenterV(vStartPos);
	glm::vec2 EndGridCenterPos   = m_pScene->getSceneGridCenterV(vEndPos);
	m_pPathFindMethod->findPathV(StartGridCenterPos, EndGridCenterPos, voResultSet);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::__chooseVisibleRoadMap(const glm::vec2& vCurrentPos, const std::vector<glm::vec2>& vRoadMap, float vVisibleRange, std::vector<glm::vec2>& voVisibleRoadMap)
{
	for (unsigned int i=0; i<vRoadMap.size(); ++i)
	{
		float Test = glm::distance(vCurrentPos, vRoadMap[i]);
		if (glm::distance(vCurrentPos, vRoadMap[i]) <= vVisibleRange)
		{
			voVisibleRoadMap.push_back(vRoadMap[i]);
		}
	}
}

////********************************************************************
////FUNCTION:
//void hiveCrowdSimulation::CCrowdSimulation::__decideGoalPosition()
//{
//	for (unsigned int i=0; i<m_AgentSet.size(); ++i)
//	{
//		if (m_NumElapsedFrame && m_AgentSet[i]->isAware() && !m_AgentSet[i]->isNecessary2FindNewGoal())
//		{
//			continue;
//		}
//		m_AgentSet[i]->decideGoalPosition();
//	}
//}

////********************************************************************
////FUNCTION:
//void hiveCrowdSimulation::CCrowdSimulation::__updateRoadMap()
//{
//	for (unsigned int i=0; i<m_AgentSet.size(); ++i)
//	{
//		if (!m_AgentSet[i]->isNeed2ComputeRoadMap())
//		{
//			continue;
//		}
//
//		std::vector<glm::vec2> FindPathResult;
//		__computeGlobalRoadMap(m_AgentSet[i]->getPosition(), m_AgentSet[i]->getGoalPosition(), FindPathResult);
//		FindPathResult.push_back(m_AgentSet[i]->getGoalPosition());
//		m_AgentSet[i]->setRoadMap(FindPathResult);
//		m_AgentSet[i]->setNeedComputeRoadMap(false);
//	}
//}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::__updatePreferVelocity()
{
	for (unsigned int i=0; i<m_AgentSet.size(); ++i)
	{
		glm::vec2 PreferVelocity = m_AgentSet[i]->computePreferVelocityCausedByTarget(g_AgentReachGoalRange);
		m_AgentSet[i]->setPreferVelocity(PreferVelocity);
	}

	for (unsigned int i=0; i<m_GroupSet.size(); ++i)
	{
		CGroup* pOneGroup = m_GroupSet[i];
		glm::vec2 GroupCenter = pOneGroup->computeGroupCenter();
		for (unsigned int k=0; k<pOneGroup->getNumGroupMembers(); ++k)
		{
			CDerivedAgent* pOneGroupMember = pOneGroup->getGroupMember(k);
			glm::vec2 VelocityByTarget     = pOneGroupMember->computePreferVelocityCausedByTarget(g_GroupMemberReachGoalRange);
			glm::vec2 VelocityByGroupForce = glm::normalize(GroupCenter - pOneGroupMember->getPosition());
			glm::vec2 PreferVelocity = VelocityByTarget * 0.7f + VelocityByGroupForce * 0.3f;
			pOneGroupMember->setPreferVelocity(PreferVelocity);
		}
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::__updateNessarity2FindNewGoal(unsigned int vElapsedFrame)
{
	for (unsigned int i=0; i<m_AgentSet.size(); ++i)
	{
		m_AgentSet[i]->updateNessarity2FindNewGoal(vElapsedFrame);
	}

	for (unsigned int k=0; k<m_GroupSet.size(); ++k)
	{
		m_GroupSet[k]->updateNessarity2FindNewGoal(vElapsedFrame);
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::setScene(hiveSceneProcess::CBaseScene* vScene)
{
	_ASSERT(vScene);
	m_pScene = vScene;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::setGraph(hiveSceneProcess::CGraphWithCrowd* vGraphWithCrowd, hiveSceneProcess::CGraphWithoutCrowd* vGraphWithoutCrowd)
{
	_ASSERT(vGraphWithCrowd && vGraphWithoutCrowd);
	m_pGraphWithCrowd    = vGraphWithCrowd;
	m_pGraphWithoutCrowd = vGraphWithoutCrowd;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::setPathFindMethod(hivePathFinding::CBasePathFinding* vPathFindMethod)
{
	_ASSERT(vPathFindMethod);
	m_pPathFindMethod = vPathFindMethod;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::initPathFindMethod()
{
	std::vector<boost::any> OtherPrepare;
	m_pPathFindMethod->init(m_pGraphWithoutCrowd, OtherPrepare);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::dumpTotalAgent(std::vector<CDerivedAgent*>& voTotalAgent) const
{
	voTotalAgent.clear();
	voTotalAgent = m_AgentSet;

	std::vector<CDerivedAgent*> OneGroupMemberSet;
	for (unsigned int i=0; i<getNumGroups(); ++i)
	{
		OneGroupMemberSet.clear();
		CGroup* pGroup = getGroup(i);
		pGroup->dumpGroupMember(OneGroupMemberSet);
		voTotalAgent.insert(voTotalAgent.end(), OneGroupMemberSet.begin(), OneGroupMemberSet.end());
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::addNoGroupAgent(hiveCrowdSimulation::CDerivedAgent* vAgent)
{
	_ASSERT(vAgent);
	m_AgentSet.push_back(vAgent);
	m_NumNoGroupAgent = m_AgentSet.size();
}

//********************************************************************
//FUNCTION:
unsigned int hiveCrowdSimulation::CCrowdSimulation::addAgent2RVOSimulatorAndReturnRVOIndex(hiveCrowdSimulation::CDerivedAgent* vAgent)
{
	_ASSERT(vAgent);
	int RVOIndex = m_pRVOSimulator->addAgent(vAgent->getInitPos());
	RVO::Agent* pRVOAgent = m_pRVOSimulator->getAgent(RVOIndex);
	pRVOAgent->setVelocity(vAgent->getInitVel());

	return RVOIndex;
}

//********************************************************************
//FUNCTION:
unsigned int hiveCrowdSimulation::CCrowdSimulation::getNumTotalAgent() const
{
	unsigned int NumGroupMembers = 0;
	for (auto& OneGroup : m_GroupSet)
	{
		NumGroupMembers += OneGroup->getNumGroupMembers();
	}
	return m_AgentSet.size() + NumGroupMembers;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulation::addGroup(CGroup* vGroup)
{
	_ASSERT(vGroup);
	m_GroupSet.push_back(vGroup);
}

//********************************************************************
//FUNCTION:
CGroup* hiveCrowdSimulation::CCrowdSimulation::getGroup(unsigned int vGroupIndex) const
{
	_ASSERT(vGroupIndex < m_GroupSet.size());
	return m_GroupSet[vGroupIndex];
}