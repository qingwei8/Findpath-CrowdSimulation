#include <memory.h>
#include <fstream>
#include "common/CommonInterface.h"
#include "common/HiveCommonMicro.h"
#include "common/HiveConfig.h"
#include "FrameWork.h"
//#include <boost/serialization/serialization.hpp>
//#include <boost/serialization/vector.hpp>
//#include <boost/archive/text_oarchive.hpp> 
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/detail/interface_oarchive.hpp>
//#include "Person.h"

void installMemoryLeakDetector()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	//_CRTDBG_LEAK_CHECK_DF: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an error 
	//report if the application failed to free all the memory it allocated. OFF: Do not automatically perform leak checking at program exit.
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//the following statement is used to trigger a breakpoint when memory leak happens
	//comment it out if there is no memory leak report;
	//_crtBreakAlloc = 135040;
#endif
}

int main()
{
	installMemoryLeakDetector();

	try
	{
		hiveCommon::hiveLoadTestingDLLs();

		int StartSecond = time(0);
		std::cout << StartSecond << std::endl;
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

		// 		glm::vec2 Vec1(3.0, 1.0);
// 		glm::vec2 Vec2(2.0, 2.0);
// 		float Test = glm::dot(Vec1, Vec2);
// 
// 		float T = glm::acos(1/2.0) * 180 / 3.1415926;
// 
// 		system("pause");
 		//hiveCrowdSimulation::CKNNSearch Test;
 		//std::ofstream File("Initial.txt");
 		//boost::archive::text_oarchive Archive(File);
 		//Archive << Test;

		/*CPerson Person1("Person", 22, glm::vec2(10, 20));
		std::vector<glm::vec2> InPositionSet;
		for (unsigned int i=0; i<3; ++i)
		{
			InPositionSet.push_back(glm::vec2(i, i));
		}
		Person1.setPosSet(InPositionSet);

		std::vector<std::vector<glm::vec2>> TwoDimensionDataSet;
		for (unsigned int i=0; i<2; ++i)
		{
			TwoDimensionDataSet.push_back(InPositionSet);
		}
		Person1.setTwoDimensionalSet(TwoDimensionDataSet);

		std::ostringstream OS;
		boost::archive::binary_oarchive OA(OS);
		OA << Person1;

		std::string Content = OS.str();

		CPerson Person2;
		std::istringstream IS(Content);
		boost::archive::binary_iarchive IA(IS);
		IA >> Person2;

		glm::vec2 Position = Person2.getPos();
		std::vector<glm::vec2> OutPositionSet = Person2.getPosSet();
		std::vector<std::vector<glm::vec2>> OutTwoDimensionalData = Person2.getTwoDimensionalSet();
		std::cout << Person2.getName() << " " << Person2.getAge() << std::endl;*/

	}
	catch (...)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The program is terminated due to unexpected error.");
	}

	return 0;
}