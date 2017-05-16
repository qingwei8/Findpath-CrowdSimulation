#pragma once
#include "BasePathFinding.h"

namespace hiveSceneProcess
{
	class CBaseScene;
}

namespace hivePathFinding
{
	class CVisionBasedPathFinding : public CBasePathFinding
	{
	public:
		CVisionBasedPathFinding(void);
		~CVisionBasedPathFinding(void);

		virtual float findPathV(const glm::vec2& vGridCenterStart, const glm::vec2& vGridCenterEnd, std::vector<glm::vec2>& vGridCenterRoadMap) override;

	protected:
		virtual void _extraInitV(const std::vector<boost::any>& vExtraParament) override;

	private:
		hiveSceneProcess::CBaseGraph* m_pGraphWithCorwd;
		hiveSceneProcess::CBaseScene* m_pScene;
		int m_VisionRadius;
	};
}