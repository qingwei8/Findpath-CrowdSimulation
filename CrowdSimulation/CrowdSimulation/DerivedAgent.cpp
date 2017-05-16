#include "DerivedAgent.h"
#include <cmath>
#include <map>
#include "AgentConfig.h"
#include "SimulationCommon.h"
#include "CrowdSimulationConfig.h"
#include "AgentConfig.h"
#include "MathHelper.h"
#include "SceneGrid.h"

using namespace hiveCrowdSimulation;

CDerivedAgent::CDerivedAgent(const glm::vec2& vPosition, const glm::vec2& vDirection) : CBaseAgent(vPosition, vDirection)
{
	m_pScene = NULL;
	m_IsNessary2FindNewGoal    = false;
	m_IsNessary2ComputeRoadMap = false;
	m_AdvanceFrameTimes = 1;
	m_NeighborSize = 0;
}

CDerivedAgent::~CDerivedAgent()
{
	for (unsigned int i=0; i<m_ConcernedAgentSet.size(); ++i)
	{
		if (m_ConcernedAgentSet[i])
		{
			delete m_ConcernedAgentSet[i];
			m_ConcernedAgentSet[i] = NULL;
		}
	}
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveCrowdSimulation::CDerivedAgent::__computeInfluenceSpace4Ignorance()
{
	float CombineWeight       = __decideCombinateWeight();
	int AdvanceFrameTime      = g_TempGoalAdvanceFrameTime / getAdvanceFrameTiems();
	glm::vec2 PrefVelocityDir =  __computeNeighborInfluence() * CombineWeight + __decideGoalDirByIgnorance() * (1 - CombineWeight);
	return getPosition() + glm::normalize(PrefVelocityDir) * (float)AdvanceFrameTime;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CDerivedAgent::__repairInfluenceNeighborSet()
{
	_ASSERT(m_NeighborPosSet.size() == m_NeighborVelSet.size() && m_NeighborIsAwareSet.size() == m_NeighborMoveSpeedSet.size() && m_NeighborPosSet.size() == m_NeighborIsAwareSet.size());

	unsigned int NumNeighbors = g_MaxNeighbors;
	while (m_NeighborPosSet.size() < NumNeighbors)
	{
		m_NeighborPosSet.push_back(glm::vec2(0.0, 0.0));
		m_NeighborVelSet.push_back(glm::vec2(0.0, 0.0));
		/*m_NeighborIsAwareSet.push_back(isAware());*///Todo:
		m_NeighborIsAwareSet.push_back(false);
		float MaxSpeed = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::MAX_SPEED);
		float NeighborSpeed = isAware() ? MaxSpeed : hiveCommon::hiveGenRandomFloat(0, MaxSpeed);
		m_NeighborMoveSpeedSet.push_back(NeighborSpeed);
	}

	if (m_NeighborPosSet.size() > NumNeighbors)
	{
		std::vector<glm::vec2> NearNeighborPosSet, NearNeighborVelSet;
		std::vector<bool> NearNeighborAwareSet;
		std::vector<float> NearNeighborSpeedSet;
		__dumpNearestNeighbor(NumNeighbors, NearNeighborPosSet, NearNeighborVelSet, NearNeighborAwareSet, NearNeighborSpeedSet);
		setNeighborInfo(NearNeighborPosSet, NearNeighborVelSet, NearNeighborAwareSet, NearNeighborSpeedSet);
	}

	m_NeighborSize = m_NeighborPosSet.size();
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CDerivedAgent::__dumpNearestNeighbor(unsigned int vNumNearestNeighbor, std::vector<glm::vec2>& voNearNeighborPos, std::vector<glm::vec2>& voNearNeighborVel, std::vector<bool>& voNearNeighborAwareness, std::vector<float>& voNearNeighborSpeed)
{
 	voNearNeighborPos.clear();
 	voNearNeighborVel.clear();
 
 	std::map<float,  unsigned int> Distance2IndexMap;
 	for (unsigned int i=0; i<m_NeighborPosSet.size(); ++i)
 	{
 		float Distance = glm::distance(getPosition(), m_NeighborPosSet[i]);
 		//Distance2IndexMap[Distance] = i;
 		//Distance2IndexMap.insert(std::pair<float, unsigned int>(Distance, i));
 		Distance2IndexMap.insert(std::make_pair(Distance, i));
 	}

	for (auto& Iter : Distance2IndexMap)
	{
		if (voNearNeighborPos.size() < vNumNearestNeighbor)
		{
			voNearNeighborPos.push_back(m_NeighborPosSet[Iter.second]);
			voNearNeighborVel.push_back(m_NeighborVelSet[Iter.second]);
			voNearNeighborAwareness.push_back(m_NeighborIsAwareSet[Iter.second]);
			voNearNeighborSpeed.push_back(m_NeighborMoveSpeedSet[Iter.second]);
		}
	}
}

//********************************************************************
//FUNCTION:
Eigen::MatrixXd hiveCrowdSimulation::CDerivedAgent::__constructInfluenceMatrix()
{
	Eigen::MatrixXd InfluenceMatrix(2, m_NeighborPosSet.size());
	for (unsigned int i=0; i<m_NeighborPosSet.size(); ++i)
	{
		glm::vec2 OneCol = __computeMatrixCol(i);

		InfluenceMatrix(0, i) = OneCol.x;
		InfluenceMatrix(1, i) = OneCol.y;
	}

	return InfluenceMatrix;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CDerivedAgent::setNeighborInfo(const std::vector<glm::vec2>& vNeighborPosSet, const std::vector<glm::vec2>& vNeighborVelSet, const std::vector<bool>& vNeighborAwareSet, std::vector<float>& vSpeedSet)
{
	_ASSERT(m_NeighborPosSet.size() == m_NeighborVelSet.size() && m_NeighborIsAwareSet.size() == m_NeighborMoveSpeedSet.size() && m_NeighborPosSet.size() == m_NeighborIsAwareSet.size());

	m_NeighborPosSet.clear();
	m_NeighborVelSet.clear();
	m_NeighborIsAwareSet.clear();
	m_NeighborMoveSpeedSet.clear();

	m_NeighborPosSet       = vNeighborPosSet;
	m_NeighborVelSet       = vNeighborVelSet;
	m_NeighborIsAwareSet   = vNeighborAwareSet;
	m_NeighborMoveSpeedSet = vSpeedSet;

	m_NeighborSize = vNeighborPosSet.size();
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveCrowdSimulation::CDerivedAgent::__computeMatrixCol(unsigned int vNeighborIndex)
{
  	glm::vec2 PosNeighbor2Agent = glm::normalize(m_NeighborPosSet[vNeighborIndex] - getPosition());
  	return __computeNeighborInfluenceWeight(vNeighborIndex) * PosNeighbor2Agent;
}

//********************************************************************
//FUNCTION:
float hiveCrowdSimulation::CDerivedAgent::__computeNeighborInfluenceWeight(unsigned int vNeighborIndex)
{
	glm::vec2 SumVelocity;
	for (unsigned int i=0; i<m_NeighborVelSet.size(); ++i)
	{
		SumVelocity += m_NeighborVelSet[i];
	}

	float InfluenceFactor = m_NeighborIsAwareSet[vNeighborIndex] ? g_AwarenessG : g_IgnoranceG;
	return InfluenceFactor * __computeDistanceInfluence(vNeighborIndex) * __computeVelocityInfluence(SumVelocity, vNeighborIndex) * __computePositionInfluence(SumVelocity, vNeighborIndex);
}

//********************************************************************
//FUNCTION:
float hiveCrowdSimulation::CDerivedAgent::__computeDistanceInfluence(unsigned int vNeighborIndex)
{
	glm::vec2 RelativePos = getPosition() -  m_NeighborPosSet[vNeighborIndex];
	return g_ConstantA * glm::length(RelativePos);
}

//********************************************************************
//FUNCTION:
float hiveCrowdSimulation::CDerivedAgent::__computeVelocityInfluence(const glm::vec2& vAvgVelocity, unsigned int vNeighborIndex)
{
	float VelDistance = glm::length(m_NeighborVelSet[vNeighborIndex] - vAvgVelocity);
	return exp(-1 * VelDistance / g_ConstantB);
}

//********************************************************************
//FUNCTION:
float hiveCrowdSimulation::CDerivedAgent::__computePositionInfluence(const glm::vec2& vAvgVelocity, unsigned int vNeighborIndex)
{
	glm::vec2 RelativePos = m_NeighborPosSet[vNeighborIndex] - getPosition();
	glm::vec2 Difference    = glm::normalize(RelativePos) - glm::normalize(vAvgVelocity);
	return exp(-1 * glm::length(Difference) / g_ConstantC);
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveCrowdSimulation::CDerivedAgent::__decideGoalDirByIgnorance()
{
	hiveSceneProcess::CSceneGrid* pCurrentGrid = m_pScene->getSceneGridV(getPosition());
	std::vector<const hiveSceneProcess::SRectShape*> AdjPassableSet;
	m_pScene->dumpAdjacentPassableSetV(pCurrentGrid->getRect()->getCenter(), AdjPassableSet);
	_ASSERT(AdjPassableSet.size() > 0);

	int ChooseIndex            = hiveCommon::hiveGenRandomInt(0, AdjPassableSet.size() - 1); 
	glm::vec2 ChooseGridCenter = AdjPassableSet[ChooseIndex]->getCenter();
	return glm::normalize(ChooseGridCenter - getPosition());
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CDerivedAgent::_setScene(const hiveSceneProcess::CBaseScene* vScene)
{
	_ASSERT(vScene);

	m_pScene = vScene;
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveCrowdSimulation::CDerivedAgent::__computeExitSignInfluence()
{
 	glm::vec2 ChooseEixt = __chooseOneDirVisibleEixt();
 	return glm::normalize(ChooseEixt - getPosition());
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveCrowdSimulation::CDerivedAgent::__chooseOneDirVisibleEixt()
{
 	_ASSERT(m_pScene);
 
 	std::vector<float> ReachPossabilitySet;
 	std::vector<unsigned int> ExitSet;
 	for (unsigned int i=0; i<m_pScene->getNumExit(); ++i)
 	{
 		bool ConditionA = glm::distance(getPosition(), m_pScene->getExit(i)) < CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::VISIBLE_RANGE);
 		bool ConditionB = m_pScene->isDirectVisibleV(getPosition(), m_pScene->getExit(i));
 		if (ConditionA && ConditionB)
 		{
 			ReachPossabilitySet.push_back(1.0 / glm::distance(getPosition(), m_pScene->getExit(i)));
 			ExitSet.push_back(i);
 		}
 	}
 
 	if (ReachPossabilitySet.size() == 0)
 	{
 		return glm::vec2(0.0, 0.0);
 	}
 
 	int ChooseIndex = exeRoulette(ReachPossabilitySet);
	_ASSERT(ChooseIndex >= 0);
 	
 	return m_pScene->getExit(ExitSet[ChooseIndex]);
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveCrowdSimulation::CDerivedAgent::__chooseOneRandomExit()
{
	_ASSERT(m_pScene);

	std::vector<float> ReachPossabilitySet;
	std::vector<unsigned int> ExitSet;
	for (unsigned int i=0; i<m_pScene->getNumExit(); ++i)
	{
		ReachPossabilitySet.push_back(1.0 / glm::distance(getPosition(), m_pScene->getExit(i)));
		ExitSet.push_back(i);
	}

	int ChooseIndex = exeRoulette(ReachPossabilitySet);
	_ASSERT(ChooseIndex >= 0);

	return m_pScene->getExit(ExitSet[ChooseIndex]);
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CDerivedAgent::__isAnyExitVisible() const
{
 	for (unsigned int i=0; i<m_pScene->getNumExit(); ++i)
 	{
 		bool ConditionA = glm::distance(getPosition(), m_pScene->getExit(i)) < CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::VISIBLE_RANGE);
 		bool ConditionB = m_pScene->isDirectVisibleV(getPosition(), m_pScene->getExit(i));
 		if (ConditionA && ConditionB)
 		{
 			return true;
 		}
 	}
 	return false;
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveCrowdSimulation::CDerivedAgent::__computeNeighborInfluence()
{
	if (m_NeighborPosSet.size() < g_MaxNeighbors / 2)//Todo:邻居个数很少时，影响空间的计算结果常常令人走到场景边缘，甚至走出场景。
	{
		return glm::vec2(0.0, 0.0);
	}

 	__repairInfluenceNeighborSet();
 	Eigen::MatrixXd InfluenceMatrix = __constructInfluenceMatrix();
	Eigen::VectorXd EigenVector = computeEigenVector(InfluenceMatrix);
 	return glm::vec2(EigenVector.x(), EigenVector.y());
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CDerivedAgent::decideGoalPosition()
{
	glm::vec2 NewGoalPosition;
	bool IsExitFound = checkAroundExit(NewGoalPosition);

	if (IsExitFound)
	{
		if (isAware() && !isNecessary2FindNewGoal())
		{
			setGoalPosition(NewGoalPosition);
			setNeedComputeRoadMap(true);
			return;
		}

		if (!isAware() && isExitSet2OldGoal())
		{
			return;
		}

		if (!isAware() && !isExitSet2OldGoal())
		{
			setGoalPosition(NewGoalPosition);
			setNeedComputeRoadMap(true);
			return;
		}
	}

	if (!isAware() && !isReachedGoal(g_AgentReachGoalRange))
	{
		return;
	}

	//Todo:执行后续代码意味着人长时间未达到原定目标，需要重新确定新目标。那么这时候不会考虑将可见的出口设置为其新目标。考虑特殊情况：人卡在路上，一直未到达其目标（人卡住的点与其目标在可视范围之内。）
	//否则，人会一直以那个他看得见，到不了的点作为目标，导致一直无法到达。
	bool NeedReDecideNewGoal;
	do 
	{
		NeedReDecideNewGoal = false;

		NewGoalPosition = isAware() ? __chooseOneRandomExit() : __computeInfluenceSpace4Ignorance();

		if (!m_pScene->isPositionInScene(NewGoalPosition))
		{
			NeedReDecideNewGoal = true;
			addAdvanceFrameTimes();
		}
		if (m_pScene->isPositionInScene(NewGoalPosition) && m_pScene->isPosInObstacle(NewGoalPosition) || (NewGoalPosition == getGoalPosition()))
		{
			NeedReDecideNewGoal = true;
			addAdvanceFrameTimes();
		}
	} while (NeedReDecideNewGoal);

	setGoalPosition(NewGoalPosition);
	setNeedComputeRoadMap(true);
	resetAdvanceFrameTimes();
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveCrowdSimulation::CDerivedAgent::decideGoalPos()
{
	glm::vec2 OldGoalPos = getGoalPosition();
	glm::vec2 NewGoalPosition;
	do 
	{
		if (isAware())
		{
			NewGoalPosition = __chooseOneRandomExit();
		}
		else
		{
			NewGoalPosition = __computeInfluenceSpace4Ignorance();
		}
	} while (m_pScene->isPosInObstacle(NewGoalPosition) && (NewGoalPosition != OldGoalPos));//人若在规定时间内未到达目标点（哪怕目标点在其可视范围内），则会重新选择除此目标点外的其他出口作为其新的目标点。
	
	return NewGoalPosition;
}

//********************************************************************
//FUNCTION:
float hiveCrowdSimulation::CDerivedAgent::__computeVelocityMagnitude()
{
	if (isAware())
	{
		return CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::MAX_SPEED);
	}

 	if (m_NeighborMoveSpeedSet.size() == 0)
 	{
 		return getMoveSpeed();
 	}

	float AvgSpeed = 0.0;
	for (unsigned int i=0; i<m_NeighborMoveSpeedSet.size(); ++i)
	{
		AvgSpeed += m_NeighborMoveSpeedSet[i];
	}
	return AvgSpeed / m_NeighborMoveSpeedSet.size();
}

//********************************************************************
//FUNCTION:
float hiveCrowdSimulation::CDerivedAgent::__decideCombinateWeight()
{
	return __getNumAwareNeighbor() / (float)g_MaxNeighbors;
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CDerivedAgent::updateFinishStateV(float vRadiusOfGoal)
{
	_ASSERT(m_pScene);
	bool IsReachedGoal = false;
	for (unsigned int i=0; i<m_pScene->getNumExit(); ++i)
	{
		//float Test = glm::distance(m_pScene->getExit(i), getPosition());
		//std::cout << getRVOIndex() << " " << Test << std::endl;

		if (glm::distance(m_pScene->getExit(i), getPosition()) <= vRadiusOfGoal)
		{
			IsReachedGoal = true;
			setReachedExit();
			setGoalPosition(m_pScene->getExit(i));
		}
	}
	return IsReachedGoal;
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CDerivedAgent::checkAroundExit(glm::vec2& voExitPos) const
{
	float VisuableRange = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::VISIBLE_RANGE);
	
	int   ExitIndex   = -1;
	float MinDistance = VisuableRange;
	for (unsigned int i=0; i<m_pScene->getNumExit(); ++i)
	{
		bool  ConditionB    = m_pScene->isDirectVisibleV(getPosition(), m_pScene->getExit(i));
		float Distance2Exit = glm::distance(getPosition(), m_pScene->getExit(i));
		bool  CondistionA   = Distance2Exit < MinDistance;
		if (ConditionB && CondistionA)
		{
			ExitIndex = i;
			MinDistance = Distance2Exit;
		}
	}

	if (ExitIndex != -1)
	{
		voExitPos = m_pScene->getExit(ExitIndex);
		return true;
	}

	return false;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CDerivedAgent::updateNessarity2FindNewGoal(unsigned int vElapsedFrame)
{
	unsigned int GiveupTime = isAware() ? CCrowdSimulationConfig::getInstance()->getAttribute<int>(SIMULATION_CONFIG_KEYWORD::AWARENESS_AGENT_GIVEUP_TIME) : CCrowdSimulationConfig::getInstance()->getAttribute<int>(SIMULATION_CONFIG_KEYWORD::IGNORANT_AGENT_GIVEUP_TIME);
	bool ConditionA = !(vElapsedFrame % GiveupTime);
	bool ConditionB = !isReachedGoal(CCrowdSimulationConfig::getInstance()->getAttribute<float>(SIMULATION_CONFIG_KEYWORD::RADIUS_OF_GOAL));
	if (ConditionA && ConditionB)
	{
		m_IsNessary2FindNewGoal = true;
	}
	else
	{
		m_IsNessary2FindNewGoal = false;
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CDerivedAgent::addConcernedAgentInfo(SConcernedAgentInfo* vInfo)
{
	_ASSERT(vInfo);
	m_ConcernedAgentSet.push_back(vInfo);
}

//********************************************************************
//FUNCTION:
int hiveCrowdSimulation::CDerivedAgent::__getNumAwareNeighbor() const
{
	int NumAwareNeighbor = 0;
	for (unsigned int i=0; i<m_NeighborIsAwareSet.size(); ++i)
	{
		if (m_NeighborIsAwareSet[i])
		{
			NumAwareNeighbor++;
		}
	}
	return NumAwareNeighbor;
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CDerivedAgent::isExitSet2OldGoal() const
{
	for (unsigned int i = 0; i < m_pScene->getNumExit(); i++)
	{
		if (getGoalPosition() == m_pScene->getExit(i))
			return true;
	}
	return false;
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveCrowdSimulation::CDerivedAgent::computePreferVelocityCausedByTarget(float vReachTempGoalDis)
{
	glm::vec2 PreferVelocity;
	if (isReachedExit())
	{
		if (!isReachedGoal(vReachTempGoalDis))
		{
			PreferVelocity = glm::normalize(getGoalPosition() - getPosition());
			return PreferVelocity;
		}
	}

	if (isReachedTempGoal(vReachTempGoalDis))
	{
		eraseFirstRoadMapPoint();
	}

	if (!isRoadMapEmpty())
	{
		PreferVelocity = glm::normalize(getFirstRoadMapPoint() - getPosition());
	}
	return PreferVelocity;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CDerivedAgent::initAgentExtraInfo(unsigned int vRVOIndex, RVO::Agent* vRVOAgent, float vMoveSpeed, const glm::vec2& vGoalPos, hiveSceneProcess::CBaseScene* vScene)
{
	setRVOIndex(vRVOIndex);
	setRVOAgent(vRVOAgent);
	setMoveSpeed(vMoveSpeed);
	setGoalPosition(vGoalPos);
	_setScene(vScene);
}