#pragma once
#include <map>
#include "DerivedAgent.h"
#include "Subgroup.h"
#include "SimulationExport.h"

namespace hiveCrowdSimulation
{
	class CGroup
	{
	public:
		CGroup();
		~CGroup();

		CROWD_SIMULATION_DLL_EXPORT CDerivedAgent* getGroupMember(unsigned int vIndex) const;
		CROWD_SIMULATION_DLL_EXPORT unsigned int getNumGroupMembers() const {return m_GroupMemberSet.size();}
		void setGroupSize(unsigned int vNumGroupMembers);
		void setGroupMember(unsigned int vIndex, CDerivedAgent* vGroupMember);
		void addGroupMember(CDerivedAgent* vGroupMember);
		void dumpGroupMember(std::vector<CDerivedAgent*>& voGroupMemberSet) const;
		void setRelationship(CDerivedAgent* vSrcAgent, CDerivedAgent* vDstAgent, SAgentRelationship* vRelationShip);

		void decideGoalPosition();

		void setGroupTempGoal(const glm::vec2& vTempGoal) {m_GroupTempGoal = vTempGoal;}
		glm::vec2 getGroupTempGoal() const {return m_GroupTempGoal;}
		glm::vec2 computeGroupCenter() const;

		bool updateFinishState(float vRadiusOfGoal);
		void updateNessarity2FindNewGoal(unsigned int vElapsedFrame);
		bool isNecessary2FindNewGoal() const {return m_IsNecessary2FindNewGoal;}

	private:
		CDerivedAgent* __getOneAwareGroupMember() const;
		CDerivedAgent* __getRandomGroupMember()   const;

	private:
		std::vector<CDerivedAgent*> m_GroupMemberSet;
		std::vector<hiveSceneProcess::CSubgroup*> m_SubgroupSet;
		std::map<CDerivedAgent*, unsigned int>        m_IndexMap;
		std::vector<std::vector<SAgentRelationship*>> m_AgentRelationshipSet;

		glm::vec2 m_GroupTempGoal;
		bool m_IsNecessary2FindNewGoal;
	};
}