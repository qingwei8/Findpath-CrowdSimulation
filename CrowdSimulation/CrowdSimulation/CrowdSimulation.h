#pragma once
#include "BaseScene.h"
#include "RVOSimulator.h"
#include "DerivedAgent.h"
#include "Group.h"
#include "KNNSearch.h"
#include "GraphWithCrowd.h"
#include "GraphWithoutCrowd.h"
#include "SimulationExport.h"
#include "BasePathFinding.h"
#include <boost/archive/detail/interface_oarchive.hpp>
#include <boost/archive/detail/interface_iarchive.hpp>

namespace hiveCrowdSimulation
{
	class CBaseScene;

	class CROWD_SIMULATION_DLL_EXPORT CCrowdSimulation
	{
	public:
		CCrowdSimulation();
		~CCrowdSimulation();

		bool isFinished();
		
		void advance();
		void initSimulator();
		void addObstacleToRVO();
		void setScene(hiveSceneProcess::CBaseScene* vScene);
		void setGraph(hiveSceneProcess::CGraphWithCrowd* vGraphWithCrowd, hiveSceneProcess::CGraphWithoutCrowd* vGraphWithoutCrowd);
		void setPathFindMethod(hivePathFinding::CBasePathFinding* vPathFindMethod);
		void initPathFindMethod();

		void dumpTotalAgent(std::vector<CDerivedAgent*>& voTotalAgent) const;
		void dumpNoGroupAgent(std::vector<CDerivedAgent*>& voAgentSet) const {voAgentSet   = m_AgentSet;}
		void dumpGroupAgent(std::vector<CGroup*>& voGroupAgent)        const {voGroupAgent = m_GroupSet;}
		void addGroup(CGroup* vGroup);
		void addNoGroupAgent(hiveCrowdSimulation::CDerivedAgent* vAgent);
		unsigned int addAgent2RVOSimulatorAndReturnRVOIndex(hiveCrowdSimulation::CDerivedAgent* vAgent);
		unsigned int getNumGroups()                const {return m_GroupSet.size();}
		unsigned int getNumTotalAgent()            const;
		RVO::RVOSimulator* getRVOSimulator()       const {return m_pRVOSimulator;}
		CGroup* getGroup(unsigned int vGroupIndex) const;

		hiveSceneProcess::CGraphWithCrowd* getGraphWithCrowdDensity()      const {return m_pGraphWithCrowd;}
		hiveSceneProcess::CGraphWithoutCrowd* getGraphWithNoCrowdDensity() const {return m_pGraphWithoutCrowd;}
		
	private:
		void __addObstacleToRVO();
		void __updateNeighbors();
		void __findNeighborsInfo(const glm::vec2& vPos, std::vector<glm::vec2>& voNeighborPosSet, std::vector<glm::vec2>& voNeighborVelSet, std::vector<bool>& voNeighborAwareSet, std::vector<float>& voSpeedSet);
		void __decideGoalDirectionAndUpdateRoadMap();
		void __computeGlobalRoadMap(const glm::vec2& vStartPos, const glm::vec2& vEndPos, std::vector<glm::vec2>& voResultSet);
		void __chooseVisibleRoadMap(const glm::vec2& vCurrentPos, const std::vector<glm::vec2>& vRoadMap, float vVisibleRange, std::vector<glm::vec2>& voVisibleRoadMap);
		//void __decideGoalPosition();
		//void __updateRoadMap();
		void __updatePreferVelocity();
		void __updateNessarity2FindNewGoal(unsigned int vElapsedFrame);

	private:
		CKNNSearch*                   m_pNeighborSearch;
		RVO::RVOSimulator*            m_pRVOSimulator;
		hiveSceneProcess::CBaseScene* m_pScene;
		hiveSceneProcess::CGraphWithCrowd*    m_pGraphWithCrowd;
		hiveSceneProcess::CGraphWithoutCrowd* m_pGraphWithoutCrowd;
		hivePathFinding::CBasePathFinding*    m_pPathFindMethod;

		std::vector<CDerivedAgent*>   m_AgentSet;
		std::vector<CGroup*>          m_GroupSet;
		unsigned int m_NumNoGroupAgent;
		unsigned int m_NumElapsedFrame;

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & m_pNeighborSearch;
			ar & m_pRVOSimulator;
			ar & m_pScene;
			ar & m_pGraphWithCrowd;
			ar & m_pGraphWithoutCrowd;

			ar & m_NumNoGroupAgent;
			if (m_AgentSet.empty()) m_AgentSet.resize(m_NumNoGroupAgent);
			for (unsigned int i=0; i<m_AgentSet.size(); ++i)
			{
				ar & m_AgentSet[i];
			}
		}
	};
}