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
void dispalyScene(const hiveSceneProcess::CUniformGridScene* vScene)
{
	hiveCrowdRendering::CVisualizationByOpenCV* pDisplay = new hiveCrowdRendering::CVisualizationByOpenCV(vScene->getSceneWidthInPixel(), vScene->getSceneHeightInPixel(),  cv::Scalar(255, 255, 0));
	pDisplay->clearScene();

	std::vector<const hiveSceneProcess::SRectShape*> ObstacleSet, PassableSet;
	vScene->dumpObstacleSet(ObstacleSet);
	vScene->dumpPassableSet(PassableSet);

	glm::vec3 PassColor(255, 255, 255);
	glm::vec3 ObstacleColor(255, 255, 0);
 
   	for (auto& Rect : ObstacleSet)
   	{
   		pDisplay->drawRectangle(Rect->m_LeftUp, Rect->m_RightDown, ObstacleColor, true);
   	}
 	for (auto& Rect : PassableSet)
 	{
 		pDisplay->drawRectangle(Rect->m_LeftUp, Rect->m_RightDown, PassColor, false);
 	}

	int Test = vScene->getNumExit();
	for (unsigned int i=0; i<vScene->getNumExit(); ++i)
	{
		pDisplay->drawPoint(vScene->getExit(i), 4.0, glm::vec3(255, 0, 0), true);
	}

	pDisplay->display();

	delete pDisplay;
}

int main()
{
	installMemoryLeakDetector();

	try 
	{
		hiveCommon::hiveLoadTestingDLLs();

		int Width = 32;
		int Height = 32;
		std::string SceneImage = "../ScenePic/Scene_512_512.bmp";
		hiveSceneProcess::CUniformGridScene* pUniformGridScene = new hiveSceneProcess::CUniformGridScene();
		pUniformGridScene->setParameter(Width, Height);
		pUniformGridScene->generateScene(SceneImage);

		int NumEixt = pUniformGridScene->getNumExit();
		dispalyScene(pUniformGridScene);

		//pUniformGridScene->testFunction();
		delete pUniformGridScene;

		system("pause");
		return 0;

	}
	catch(...)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The program is terminated due to unexpected error.");
	}
}