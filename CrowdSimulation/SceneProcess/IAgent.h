#pragma once
#include "Agent.h"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include "SceneExport.h"
#include "BaseScene.h"

namespace hiveSceneProcess
{
	const float g_ReachTempGoalThrehold = 5.0;

	class SCENE_DLL_EXPORT CBaseAgent
	{
	public:
		CBaseAgent(const glm::vec2& vPosition, const glm::vec2& vDirection);
		virtual ~CBaseAgent();

		void setVelocity(const glm::vec2& vVelocity);
		void setPreferVelocity(const glm::vec2& vPreferredVelocity);
		void setPosition(const glm::vec2& vPos);
		void setMaxSpeed(float vSpeed);
		void setGroupID(unsigned int vID);
		void setRVOIndex(int vIndex) {m_RVOIndex = vIndex;}
		void setRVOAgent(RVO::Agent* vAgent);
		void setMoveSpeed(float vSpeed);
		void setGoalPosition(const glm::vec2& vGoalPos);

		int getRVOIndex()           const {return m_RVOIndex;}  
		float getMoveSpeed()        const {return m_pRVOAgent->getMoveSpeed();}
		glm::vec2 getInitPos()      const {return m_InitPos;}
		glm::vec2 getInitVel()      const {return m_InitDir;}
		glm::vec2 getPosition()     const {return m_pRVOAgent->getPosition();}
		glm::vec2 getVelocity()     const {return m_pRVOAgent->getVelocity();}
		glm::vec2 getGoalPosition() const {return m_pRVOAgent->getGoalPos();}
		unsigned int getGroupID()   const {return m_pRVOAgent->getGroupID();}

		void setIgnorance();
		bool isAware() const;
		bool isReachedTempGoal(float vReachTempGoalDist) const;

		void setRoadMap(const std::vector<glm::vec2>& vRoadMapSet) {m_RoadMapSet = vRoadMapSet; m_RoadMapSize = m_RoadMapSet.size();}
		void eraseFirstRoadMapPoint();
		glm::vec2 getFirstRoadMapPoint() const {_ASSERT(m_RoadMapSet.size() > 0); return m_RoadMapSet[0];}
		
		void setReachedExit() {m_IsReachedExit = true;}
		bool isReachedExit() const {return m_IsReachedExit;}

		void setNecessary2UpdateRoadMap(bool vState) {m_IsNecessary2UpdateRoadMap = vState;}
		bool isNecessary2UpdateRoadMap()       const {return m_IsNecessary2UpdateRoadMap;}
		bool isReachedGoal(float vDistanceRange);
		virtual bool updateFinishStateV(float vRadiusOfGoal) {return false;}
		bool isRoadMapEmpty() const {return m_RoadMapSet.size() == 0;}
		void dumpRoadMap(std::vector<glm::vec2>& voRoadMap) const {voRoadMap = m_RoadMapSet;} 

	protected:

	private:
		RVO::Agent* m_pRVOAgent;
		glm::vec2   m_InitPos;
		glm::vec2   m_InitDir;
		std::vector<glm::vec2> m_RoadMapSet;
		unsigned int m_RoadMapSize;
		int         m_RVOIndex;
		bool        m_IsRVOAgentSpecified;
		bool        m_IsReachedExit;
		bool        m_IsNecessary2UpdateRoadMap;

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int)
		{
			ar & m_pRVOAgent;
			ar & m_InitPos.x;
			ar & m_InitPos.y;
			ar & m_InitDir.x;
			ar & m_InitDir.y;
			ar & m_RVOIndex;
			ar & m_IsRVOAgentSpecified;
			ar & m_IsReachedExit;

			ar& m_RoadMapSize;
			if (m_RoadMapSet.empty()) m_RoadMapSet.resize(m_RoadMapSize);
			for (unsigned int i=0; i<m_RoadMapSet.size(); ++i)
			{
				ar & m_RoadMapSet[i].x;
				ar & m_RoadMapSet[i].y;
			}
		}
	};
}