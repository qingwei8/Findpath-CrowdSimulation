#include <memory.h>
#include "common/CommonInterface.h"
#include "common/HiveCommonMicro.h"
#include "common/HiveConfig.h"
#include "FrameWork.h"

void installMemoryLeakDetector()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	//_CRTDBG_LEAK_CHECK_DF: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an error 
	//report if the application failed to free all the memory it allocated. OFF: Do not automatically perform leak checking at program exit.
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//the following statement is used to trigger a breakpoint when memory leak happens
	//comment it out if there is no memory leak report;
	//_crtBreakAlloc = 267300;
#endif
}

int main()
{
	installMemoryLeakDetector();

	try
	{
		hiveCommon::hiveLoadTestingDLLs();

		int StartSecond = time(0);

 		std::string AgentInfo      = "AgentConfig.xml";
 		std::string SceneInfo      = "SceneConfig.xml";
 		std::string SimulationInfo = "SimulationConfig.xml";
		std::string ColorInfo      = "ColorConfig.xml";
 
		hiveCrowdSimulation::CFrameWork::getInstance()->initColor(ColorInfo);
 		hiveCrowdSimulation::CFrameWork::getInstance()->initFramework(AgentInfo, SceneInfo, SimulationInfo);
		hiveCrowdSimulation::CFrameWork::getInstance()->doSimulation();

		int EndSecond = time(0);
		std::cout << "花费时间： " << (EndSecond - StartSecond) << "秒" << std::endl;
		system("pause");
	}
	catch (...)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The program is terminated due to unexpected error.");
	}

	return 0;
}