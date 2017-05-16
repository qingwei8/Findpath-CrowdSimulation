#include "QuadTreeScene.h"

using namespace hiveSceneProcess;

CQuadTreeScene::CQuadTreeScene()
{
}

CQuadTreeScene::~CQuadTreeScene()
{
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CQuadTreeScene::_generateSceneV(const char* vImageData)
{
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CQuadTreeScene::dumpPassableSetInsightV(const glm::vec2& vGridCenter, float vDistance, std::vector<const SRectShape*>& voGirdSet) const 
{
	return false;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CQuadTreeScene::dumpAdjacentPassableSetV(const glm::vec2& vCurrPos, std::vector<const SRectShape*>& voAdjSet) const 
{
}

//********************************************************************
//FUNCTION:
CSceneGrid* hiveSceneProcess::CQuadTreeScene::getSceneGridV(const glm::vec2& vPosition) const 
{
	return NULL;
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveSceneProcess::CQuadTreeScene::getSceneGridCenterV(const glm::vec2& vPosInScene) const 
{
	return glm::vec2();
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CQuadTreeScene::isDirectVisibleV(const glm::vec2& vScenePosA, const glm::vec2& vScenePosB) const 
{
	return false;
}