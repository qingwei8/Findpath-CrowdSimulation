#include "BaseScene.h"
#include "SceneParser.h"
#include "SceneGrid.h"

using namespace hiveSceneProcess;

CBaseScene::CBaseScene() : m_IsDivideSceneInfoSpecified(false)
{
	m_PassableGridSize = 0;
	m_ObstacleGridSize = 0;
	m_NumExit = 0;
}

CBaseScene::~CBaseScene()
{
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseScene::setParameter(int vParameterA, int vParameterB)
{
	m_ParameterA = vParameterA;
	m_ParameterB = vParameterB;
	m_IsDivideSceneInfoSpecified = true;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseScene::generateScene(const std::string& vSceneImage)
{
	_ASSERT(!vSceneImage.empty());

	CSceneParser* pSceneParser = new CSceneParser();
	pSceneParser->parseSceneImage(vSceneImage);

	m_SceneWidthInPixel = pSceneParser->getSceneWidth();
	m_SceneHeightInPixel = pSceneParser->getSceneHeight();

	for (unsigned int i=0; i<pSceneParser->getNumExit(); ++i)
	{
		__addExit(pSceneParser->getExit(i));
	}

	_generateSceneV(pSceneParser->getImageData());

	delete pSceneParser;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseScene::__addExit(const glm::vec2& vEixt)
{
	m_ExitSet.push_back(vEixt);
	m_NumExit = m_ExitSet.size();
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseScene::dumpObstacleSet(std::vector<const SRectShape*>& voObstacleSet) const
{
	voObstacleSet = m_ObstacleGirdSet;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseScene::dumpPassableSet(std::vector<const SRectShape*>& voPassableSet) const
{
	voPassableSet = m_PassableGridSet;
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveSceneProcess::CBaseScene::getExit(unsigned int vIndex) const
{
	_ASSERT(vIndex < m_ExitSet.size());
	return m_ExitSet[vIndex];
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CBaseScene::isPositionInScene(const glm::vec2& vScenePos) const
{
	return (vScenePos.x >= 0 && vScenePos.x < getSceneWidthInPixel() && vScenePos.y >=0 && vScenePos.y < getSceneHeightInPixel());
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseScene::_addPassableGrid(CSceneGrid* vSceneGrid)
{
	m_PassableGridSet.push_back(vSceneGrid->getRect());
	m_PassableGridSize = m_PassableGridSet.size();
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CBaseScene::_addObstacleGrid(CSceneGrid* vSceneGrid)
{
	m_ObstacleGirdSet.push_back(vSceneGrid->getRect());
	m_ObstacleGridSize = m_ObstacleGirdSet.size();
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CBaseScene::isPosInObstacle(const glm::vec2& vPos) const
{
	if (!isPositionInScene(vPos))
	{
		std::cout << vPos.x << " " << vPos.y << std::endl;
	}
	_ASSERT(isPositionInScene(vPos));
	CSceneGrid* pGrid = getSceneGridV(vPos);
	return !pGrid->isPassable();
}