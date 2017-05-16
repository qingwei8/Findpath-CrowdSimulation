#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "common/CommonInterface.h"
#include "common/HiveCommonMicro.h"
#include "BaseScene.h"
#include "UniformGridScene.h"
#include "VisualizationByOpenCV.h"

void installMemoryLeakDetector()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	//_CRTDBG_LEAK_CHECK_DF: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an error 
	//report if the application failed to free all the memory it allocated. OFF: Do not automatically perform leak checking at program exit.
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//the following statement is used to trigger a breakpoint when memory leak happens
	//comment it out if there is no memory leak report;
	//_crtBreakAlloc = 12153;
#endif
}

//********************************************************************
//FUNCTION:
void dispalyScene(const hiveSceneProcess::CUniformGridScene* vScene, hiveCrowdRendering::CVisualizationByOpenCV* vDisplay)
{
	vDisplay->clearScene();

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

	int Test = vScene->getNumExit();
	for (unsigned int i=0; i<vScene->getNumExit(); ++i)
	{
		vDisplay->drawPoint(vScene->getExit(i), 4.0, glm::vec3(255, 0, 0), true);
	}
}

int main()
{
	installMemoryLeakDetector();

	try 
	{
		hiveCommon::hiveLoadTestingDLLs();

		int Width  = 32;
		int Height = 32;
		std::string SceneImage = "../ScenePic/Scene_512_512_0.bmp";
		hiveSceneProcess::CUniformGridScene* pUniformGridScene = new hiveSceneProcess::CUniformGridScene();
		pUniformGridScene->setParameter(Width, Height);
		pUniformGridScene->generateScene(SceneImage);

		int NumEixt = pUniformGridScene->getNumExit();
		hiveCrowdRendering::CVisualizationByOpenCV* pDisplay = new hiveCrowdRendering::CVisualizationByOpenCV(pUniformGridScene->getSceneWidthInPixel(), pUniformGridScene->getSceneHeightInPixel(),  cv::Scalar(255, 255, 0));
		dispalyScene(pUniformGridScene, pDisplay);

		//特殊测试1：X相同
// 		glm::vec2 ScenePosA = glm::vec2(48, 176);/*glm::vec2(1, 5);*/
// 		glm::vec2 ScenePosB = glm::vec2(48, 336);/*glm::vec2(1, 10);*/
// 		
// 		std::vector<glm::vec2> GridCenterRes;
// 		pUniformGridScene->computeGridCoordBetweenTwoGrids(ScenePosA, ScenePosB, GridCenterRes);

		//特殊测试2：Y相同
		//glm::vec2 ScenePosA = glm::vec2(48, 176);/*glm::vec2(1, 5);*/
		//glm::vec2 ScenePosB = glm::vec2(336, 176);/*glm::vec2(10, 5);*/
		//std::vector<glm::vec2> GridCenterRes;
		//pUniformGridScene->computeGridCoordBetweenTwoGrids(ScenePosA, ScenePosB, GridCenterRes);

		//特殊测试3：斜率为正
		//glm::vec2 ScenePosA = glm::vec2(38, 70);/*glm::vec2(1, 2);*/
		//glm::vec2 ScenePosB = glm::vec2(170, 202);/*glm::vec2(5, 6);*/
		//std::vector<glm::vec2> GridCenterRes;
		//pUniformGridScene->computeGridCoordBetweenTwoGrids(ScenePosA, ScenePosB, GridCenterRes);

		////特殊测试4：斜率为负
		//glm::vec2 ScenePosA = glm::vec2(40, 180);/*glm::vec2(1, 5);*/
		//glm::vec2 ScenePosB = glm::vec2(182, 42);/*glm::vec2(5, 1);*/
		//std::vector<glm::vec2> GridCenterRes;
		//pUniformGridScene->computeGridCoordBetweenTwoGrids(ScenePosA, ScenePosB, GridCenterRes);

		//5.其他情况测试
		//glm::vec2 ScenePosA = glm::vec2(16, 16);//(0, 6)
		//glm::vec2 ScenePosB = glm::vec2(144, 112);//(4, 0)
		//std::vector<glm::vec2> GridCenterRes;
		//pUniformGridScene->computeGridCoordBetweenTwoGrids(ScenePosA, ScenePosB, GridCenterRes);

		glm::vec2 ScenePosA = glm::vec2(200, 100);
		glm::vec2 ScenePosB = glm::vec2(200, 480);
		bool Visible = pUniformGridScene->isDirectVisibleV(ScenePosA, ScenePosB);

		pDisplay->drawPoint(ScenePosA, 4.0, glm::vec3(0, 255, 0), true);
		pDisplay->drawPoint(ScenePosB, 4.0, glm::vec3(0, 255, 0), true);
// 		for (unsigned int i=0; i<GridCenterRes.size(); ++i)
// 		{
// 			pDisplay->drawPoint(GridCenterRes[i], 4.0, glm::vec3(0, 255, 0), false);
// 		}
		pDisplay->display();
		delete pUniformGridScene;
		delete pDisplay;

		system("pause");
		return 0;

	}
	catch(...)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The program is terminated due to unexpected error.");
	}
}