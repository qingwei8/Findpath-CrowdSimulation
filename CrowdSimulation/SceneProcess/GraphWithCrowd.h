#pragma once
#include "BaseGraph.h"

namespace hiveSceneProcess
{
	class SCENE_DLL_EXPORT CGraphWithCrowd : public CBaseGraph
	{
	public:
		CGraphWithCrowd();
		~CGraphWithCrowd();

	protected:
		virtual float _computeEdgeWeightV(const glm::vec2& vBegin, const glm::vec2& vEnd) const override;
	};
}