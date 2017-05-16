#include <vector>
#include <glm/glm.hpp>
#include "common/CommonInterface.h"
#include "common/HiveCommonMicro.h"
#include "FrameWork.h"
#include "VisualizationByOpenCV.h"
#include "GraphWithCrowd.h"
#include "GraphWithoutCrowd.h"
#include "SceneGrid.h"

void installMemoryLeakDetector()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	//_CRTDBG_LEAK_CHECK_DF: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an error 
	//report if the application failed to free all the memory it allocated. OFF: Do not automatically perform leak checking at program exit.
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//the following statement is used to trigger a breakpoint when memory leak happens
	//comment it out if there is no memory leak report;
	//_crtBreakAlloc = 17181;
#endif
}

//********************************************************************
//FUNCTION:
void displayScene(hiveCrowdRendering::CVisualizationByOpenCV* vDisplay, const hiveSceneProcess::CBaseScene* vScene)
{
	_ASSERT(vDisplay && vScene);

	std::vector<const hiveSceneProcess::SRectShape*> ObstacleSet, PassableSet;
	vScene->dumpObstacleSet(ObstacleSet);
	vScene->dumpPassableSet(PassableSet);

	glm::vec3 PassColor(255, 255, 255);
	glm::vec3 ObstacleColor(255, 255, 0);

	for (auto& Rect : ObstacleSet)
	{
		vDisplay->drawRectangle(Rect->m_LeftUp, Rect->m_RightDown, ObstacleColor, true);
	}
	for (auto& Rect : PassableSet)
	{
		vDisplay->drawRectangle(Rect->m_LeftUp, Rect->m_RightDown, PassColor, false);
	}
}

int main()
{
	installMemoryLeakDetector();
	try
	{
		hiveCommon::hiveLoadTestingDLLs();

		std::string AgentInfo = "AgentConfig.xml";
		std::string SceneInfo = "SceneConfig.xml";
		std::string SimulationInfo = "SimulationConfig.xml";
		std::string ColorInfo = "ColorConfig.xml";

		hiveCrowdSimulation::CFrameWork::getInstance()->initColor(ColorInfo);
		hiveCrowdSimulation::CFrameWork::getInstance()->initFramework(AgentInfo, SceneInfo, SimulationInfo);

		hiveSceneProcess::CBaseScene* pScene = hiveCrowdSimulation::CFrameWork::getInstance()->getScene();
		hiveCrowdRendering::CVisualizationByOpenCV* pDisplay = new hiveCrowdRendering::CVisualizationByOpenCV(pScene->getSceneWidthInPixel(), pScene->getSceneHeightInPixel(),  cv::Scalar(255, 0, 0));
		//pDisplay->clearScene();
		//displayScene(pDisplay, pScene);

		//glm::vec2 ScenePos = glm::vec2(50, 10);
		////pDisplay->drawPoint(ScenePos, 5, glm::vec3(255, 0, 0), true);
		//hiveSceneProcess::CSceneGrid* pSceneGrid = pScene->getSceneGridV(ScenePos);
		//pDisplay->drawRectangle(pSceneGrid->getRect()->m_LeftUp, pSceneGrid->getRect()->m_RightDown, glm::vec3(255, 0, 0), true);

		//pDisplay->display();

		hiveCrowdSimulation::CCrowdSimulation* pSimulation = hiveCrowdSimulation::CFrameWork::getInstance()->getSimulation();
		hiveSceneProcess::CGraphWithoutCrowd* pGraphWithoutCrowd = pSimulation->getGraphWithNoCrowdDensity();

		//pGraphWithoutCrowd->getEdgeWeight()


		delete pDisplay;
	}
	catch (...)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The program is terminated due to unexpected error.");
	}

	return 0;
}