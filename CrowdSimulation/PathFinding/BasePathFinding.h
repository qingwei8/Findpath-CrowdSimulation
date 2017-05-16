#pragma once
#include <vector>
#include "boost/any.hpp"
#include "glm/glm.hpp"
#include "FindPathExport.h"

namespace hiveSceneProcess
{
	class CBaseGraph;
}

namespace hivePathFinding
{
	class FIND_PATH_DLL_EXPORT CBasePathFinding
	{
	public:
		CBasePathFinding(void);
		virtual ~CBasePathFinding(void);

		void init(hiveSceneProcess::CBaseGraph* vGraph, const std::vector<boost::any>& vExtraParament);
		virtual float findPathV(const glm::vec2& vGridCenterStart, const glm::vec2& vGridCenterEnd, std::vector<glm::vec2>& vGridCenterRoadMap) = 0;

	protected:
		virtual void _extraInitV(const std::vector<boost::any>& vExtraParament) {}

	protected:
		hiveSceneProcess::CBaseGraph* m_pGraph;
	};
}