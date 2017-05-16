#pragma once
#include "common/Singleton.h"
#include "BaseScene.h"
#include "CrowdSimulation.h"
#include "GraphWithCrowd.h"
#include "GraphWithoutCrowd.h"
#include "Visualization.h"
#include "SimulationExport.h"

namespace hiveCrowdSimulation
{
	class CDerivedAgent;

	class CROWD_SIMULATION_DLL_EXPORT CFrameWork : public hiveCommon::CSingleton<CFrameWork>
	{
	public:
		~CFrameWork();

		void initFramework(const std::string& vAgentInfo, const std::string& vSceneInfo, const std::string& vSimulationInfo);
	    void doSimulation();

		void initColor(const std::string& vDisplayColorInfo);
		CCrowdSimulation* getSimulation()        const {return m_pCrowdSimulation;}
		hiveSceneProcess::CBaseScene* getScene() const {return m_pScene;}

	private:
		CFrameWork();
	
		void __initSimulator();
		void __initSceneInfo();
		void __initAgentInfo();
		void __initNoGroupAgentInfoByFile(const std::string& vAgentFile);
		void __initGroupInfoByFile(const std::string& vGroupFile);
		void __initGraphInfo();
		void __initPathFindingInfo();

		CDerivedAgent* __genOneAgent();
		void __repairGenAgentRect(hiveSceneProcess::SRectShape* voRect);
		void __initCrowdDistribution(unsigned int vNumTotalAgent, unsigned int vNumIgnorantAgent, unsigned int vNumNotGroupMembers, unsigned int vNumGroups, unsigned int vMinNumGroupMembers, unsigned int vMaxNumGroupMembers);
		void __initNotGroupMemberDistribution(unsigned int vNumAgent);
		void __initGroupDistribution(unsigned int vNumAllGroupMembers, unsigned int vNumGroup, unsigned int vMinNumGroupMembers, unsigned int vMaxNumGroupMembers);
		void __decideGroupSize(int vNumAllGroupMembers, int vNumGroup, int vMinNumGroupMembers, int vMaxNumGroupMembers);
		void __initGroupMembers(CGroup* vGroup, unsigned int vGroupID);
		void __initRelationship(CGroup* vGroup);
		void __repairGroupDistanceLimit(CGroup* vGroup);
		void __limitPosInScene(glm::vec2& voPos);
		void __initIgnorantAgent(unsigned int vNumIgnorantAgent);
		
		void __initVisualizationInfo();
		void __updateVisualization();
		void __clearScene() {m_pVisualization->clearScene();}
		void __drawScene();
		void __drawExit();
		void __drawCrowd();
		void __display() {m_pVisualization->display();}
		void __recordSimulationResult();
		void __drawNoGroupAgents(const std::vector<CDerivedAgent*>& vNoGroupAgentSet, const glm::vec3& vAgentColor);
		void __drawGroupAgents(const std::vector<CGroup*>& vGroupAgentSet, const glm::vec3& vGroupColor);
		void __drawRoadMap(const std::vector<glm::vec2>& vRoadMapSet, const glm::vec3& vColor);

	private:
		bool                                  m_IsInit;
		bool                                  m_IsColorConfigParsed;
		CCrowdSimulation*                     m_pCrowdSimulation;
		hiveSceneProcess::CBaseScene*         m_pScene;
		hiveCrowdRendering::CVisualization*   m_pVisualization;

		friend class hiveCommon::CSingleton<CFrameWork>;

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & m_IsInit;
			ar & m_IsColorConfigParsed;
			ar & m_pCrowdSimulation;
			ar & m_pScene;
		}
	};
}