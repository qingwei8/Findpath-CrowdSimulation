#pragma once
#include "BaseGraph.h"

namespace hiveSceneProcess
{
	class SCENE_DLL_EXPORT CGraphWithoutCrowd : public CBaseGraph
	{
	public:
		CGraphWithoutCrowd();
		~CGraphWithoutCrowd();

	protected:
		virtual float _computeEdgeWeightV(const glm::vec2& vBegin, const glm::vec2& vEnd) const override;

	private:

	};
}