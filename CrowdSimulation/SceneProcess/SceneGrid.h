#pragma once
#include "SceneCommon.h"
#include "IAgent.h"
#include "SceneExport.h"

namespace hiveSceneProcess
{
	class SCENE_DLL_EXPORT CSceneGrid
	{
	public:
		CSceneGrid(const SRectShape* pRect, bool vIsPassable);
		~CSceneGrid();

		bool isPassable()           const {return m_IsPassable;}
		const SRectShape* getRect() const {return m_pRect;}
		unsigned int getNumAgent()  const {return m_AgentSet.size();}
		float getCrowdDensity() const;

		float     getAvgSpeed();
		glm::vec2 getAvgDirection();

		void addAgent(CBaseAgent* vAgent);
		void removeAgent(CBaseAgent* vAgent);//Todo:Œ¥ µœ÷

	private:
		void __updateCrowdAvgVelocity();
		void __updateCrowdAvgSpeed();

	private:
		bool m_IsPassable;
		bool m_VelocityUpdated;
		bool m_SpeedUpdated;
		const SRectShape* m_pRect;
		glm::vec2 m_AvgDirection;
		float m_AvgSpeed;
		std::vector<CBaseAgent*> m_AgentSet;
	};
}