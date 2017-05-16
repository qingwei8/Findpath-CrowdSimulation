#pragma once
#include "BaseScene.h"

namespace hiveSceneProcess
{
	class SCENE_DLL_EXPORT CQuadTreeScene : public CBaseScene
	{
	public:
		CQuadTreeScene();
		~CQuadTreeScene();

	public:
		virtual bool dumpPassableSetInsightV(const glm::vec2& vGridCenter, float vDistance, std::vector<const SRectShape*>& voGirdSet) const override;
		virtual void dumpAdjacentPassableSetV(const glm::vec2& vGridCenter, std::vector<const SRectShape*>& voAdjSet) const override;
		virtual CSceneGrid* getSceneGridV(const glm::vec2& vPosInScene) const override;
		virtual glm::vec2 getSceneGridCenterV(const glm::vec2& vPosInScene) const override;
		virtual bool isDirectVisibleV(const glm::vec2& vScenePosA, const glm::vec2& vScenePosB) const override;

	protected:
		virtual void _generateSceneV(const char* vImageData) override;
	};
}