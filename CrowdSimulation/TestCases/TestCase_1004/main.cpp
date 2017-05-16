#include <memory.h>
#include "common/CommonInterface.h"
#include "common/HiveCommonMicro.h"
#include "common/HiveConfig.h"
#include "FrameWork.h"
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
	//_crtBreakAlloc = 12115;
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

	for (unsigned int i=0; i<vScene->getNumExit(); ++i)
	{
		vDisplay->drawPoint(vScene->getExit(i), 5.0, glm::vec3(255, 0, 0), true);
	}
	
// 	glm::vec2 ScenePos = glm::vec2(50, 10);
// 	vDisplay->drawPoint(ScenePos, 5, glm::vec3(255, 0, 0), true);
}

//********************************************************************
//FUNCTION:
void displayCrowdDistribution(hiveCrowdRendering::CVisualizationByOpenCV* vDisplay, hiveCrowdSimulation::CCrowdSimulation* vCrowdSimulation)
{
	_ASSERT(vDisplay && vCrowdSimulation);

	std::vector<hiveCrowdSimulation::CDerivedAgent*> AgentSet;
	vCrowdSimulation->dumpNoGroupAgent(AgentSet);

	std::vector<hiveCrowdSimulation::CGroup*> GroupSet;
	vCrowdSimulation->dumpGroupAgent(GroupSet);

	glm::vec3 AgentColor = glm::vec3(0, 255, 0);
	glm::vec3 GroupColor = glm::vec3(0, 0, 255);
	for (unsigned int i=0; i<AgentSet.size(); ++i)
	{
		hiveCrowdSimulation::CDerivedAgent* pAgent = AgentSet[i];
		bool IsFilled = false;
		if (pAgent->isAware())
		{
			IsFilled = true;
		}
		vDisplay->drawPoint(pAgent->getPosition(), 4, AgentColor, IsFilled);
	}

	for (unsigned int i=0; i<GroupSet.size(); ++i)
	{
		for (unsigned int k=0; k<GroupSet[i]->getNumGroupMembers(); ++k)
		{
			bool IsFilled = false;
			hiveCrowdSimulation::CDerivedAgent* pOneGroupMember = GroupSet[i]->getGroupMember(k);
			if (pOneGroupMember->isAware())
			{
				IsFilled = true;
			}
			glm::vec2 Pos = pOneGroupMember->getPosition();
			vDisplay->drawRectangle(glm::vec2(Pos.x - 4, Pos.y - 4), glm::vec2(Pos.x + 4, Pos.y +4), GroupColor, IsFilled);
		}
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
		pDisplay->clearScene();
		displayScene(pDisplay, pScene);
		displayCrowdDistribution(pDisplay, hiveCrowdSimulation::CFrameWork::getInstance()->getSimulation());
		pDisplay->display();

		delete pDisplay;
		system("pause");
	}
	catch (...)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The program is terminated due to unexpected error.");
	}
	return 0;
}