#include "Group.h"
#include "common/CommonInterface.h"
#include "CrowdSimulationConfig.h"
#include "SimulationCommon.h"

using namespace hiveCrowdSimulation;

CGroup::CGroup()
{
	hiveSceneProcess::CSubgroup* pSubgroup = new hiveSceneProcess::CSubgroup();
	m_GroupMemberSet.resize(0);
	m_SubgroupSet.push_back(pSubgroup);
	m_GroupTempGoal = glm::vec2(-1, -1);
}

CGroup::~CGroup()
{
	for (auto& GroupMember : m_GroupMemberSet)
	{
		if (GroupMember)
		{
			delete GroupMember;
			GroupMember = NULL;
		}
	}

	for (unsigned int i=0; i<m_SubgroupSet.size(); ++i)
	{
		if (m_SubgroupSet[i])
		{
			delete m_SubgroupSet[i];
			m_SubgroupSet[i] = NULL;
		}
	}
	for (unsigned int i=0; i<m_AgentRelationshipSet.size(); ++i)
	{
		for (unsigned int k=0; k<m_AgentRelationshipSet[i].size(); ++k)
		{
			if (m_AgentRelationshipSet[i][k])
			{
				delete m_AgentRelationshipSet[i][k];
				m_AgentRelationshipSet[i][k] = NULL;
			}
		}
	}
}

//********************************************************************
//FUNCTION:
CROWD_SIMULATION_DLL_EXPORT CDerivedAgent* hiveCrowdSimulation::CGroup::getGroupMember(unsigned int vIndex) const
{
	_ASSERT(vIndex < getNumGroupMembers());
	return m_GroupMemberSet[vIndex];
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CGroup::setGroupSize(unsigned int vNumGroupMembers)
{
	m_GroupMemberSet.resize(vNumGroupMembers);
	m_AgentRelationshipSet.resize(vNumGroupMembers);
	for (unsigned int i=0; i<m_AgentRelationshipSet.size(); ++i)
	{
		m_AgentRelationshipSet[i].resize(vNumGroupMembers);
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CGroup::setGroupMember(unsigned int vIndex, CDerivedAgent* vGroupMember)
{
	_ASSERT(vIndex < getNumGroupMembers());
	m_GroupMemberSet[vIndex] = vGroupMember;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CGroup::addGroupMember(CDerivedAgent* vGroupMember)
{
	_ASSERT(vGroupMember);

	m_IndexMap.insert(std::make_pair(vGroupMember, m_GroupMemberSet.size()));
	m_GroupMemberSet.push_back(vGroupMember);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CGroup::dumpGroupMember(std::vector<CDerivedAgent*>& voGroupMemberSet) const
{
	voGroupMemberSet.clear();
	voGroupMemberSet = m_GroupMemberSet;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CGroup::setRelationship(CDerivedAgent* vSrcAgent, CDerivedAgent* vDstAgent, SAgentRelationship* vRelationShip)
{
	_ASSERT(vSrcAgent && vDstAgent && vRelationShip);
	int i = m_IndexMap[vSrcAgent];
	int k = m_IndexMap[vDstAgent];
	m_AgentRelationshipSet[i][k] = vRelationShip;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CGroup::decideGoalPosition()
{
	glm::vec2 NewGoalPosition;
	for (unsigned int i=0; i<getNumGroupMembers(); ++i)
	{
		if (m_GroupMemberSet[i]->checkAroundExit(NewGoalPosition))
		{
			setGroupTempGoal(NewGoalPosition);
			return;
		}
	}

	CDerivedAgent* pOneAwareGroupMember = __getOneAwareGroupMember();
	if (pOneAwareGroupMember == NULL)
	{
		pOneAwareGroupMember = __getRandomGroupMember();
	}
	pOneAwareGroupMember->decideGoalPosition();
	setGroupTempGoal(pOneAwareGroupMember->getGoalPosition());
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveCrowdSimulation::CGroup::computeGroupCenter() const
{
	_ASSERT(getNumGroupMembers() > 0);
	glm::vec2 GroupCenter;
	for (unsigned int i=0; i<getNumGroupMembers(); ++i)
	{
		GroupCenter += getGroupMember(i)->getPosition();
	}
	return GroupCenter / (float)getNumGroupMembers();
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CGroup::updateFinishState(float vRadiusOfGoal)
{
	bool IsGroupFinished = true;
	for (unsigned int i=0; i<getNumGroupMembers(); ++i)
	{
		if (!getGroupMember(i)->updateFinishStateV(vRadiusOfGoal))
		{
			IsGroupFinished = false;
		}
	}

	return IsGroupFinished;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CGroup::updateNessarity2FindNewGoal(unsigned int vElapsedFrame)
{
	unsigned int GroupGiveupTime = CCrowdSimulationConfig::getInstance()->getAttribute<int>(SIMULATION_CONFIG_KEYWORD::GROUP_GIVEUP_TIME);
	bool ConditionA = !(vElapsedFrame % GroupGiveupTime);

	if (!ConditionA)
	{
		m_IsNecessary2FindNewGoal = false;
		return;
	}

	for (unsigned int i=0; i<getNumGroupMembers(); ++i)
	{
		if (!getGroupMember(i)->isReachedGoal(CCrowdSimulationConfig::getInstance()->getAttribute<float>(SIMULATION_CONFIG_KEYWORD::RADIUS_OF_GOAL)))
		{
				m_IsNecessary2FindNewGoal = false;
				return;
		}
	}

	m_IsNecessary2FindNewGoal = true;
}

//********************************************************************
//FUNCTION:
CDerivedAgent* hiveCrowdSimulation::CGroup::__getOneAwareGroupMember() const
{
	for (unsigned int i=0; i<getNumGroupMembers(); ++i)
	{
		if (m_GroupMemberSet[i]->isAware())
		{
			return m_GroupMemberSet[i];
		}
	}
	return NULL;
}

//********************************************************************
//FUNCTION:
CDerivedAgent* hiveCrowdSimulation::CGroup::__getRandomGroupMember() const
{
// 	int Index = hiveCommon::hiveGenRandomInt(0, m_NumGroupMembers);
// 	return getGroupMember(Index);

	return getGroupMember(0);
}