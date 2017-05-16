#include <vector>
#include <glm/glm.hpp>
#include "common/CommonInterface.h"
#include "common/HiveCommonMicro.h"
#include "KNNSearch.h"

void installMemoryLeakDetector()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	//_CRTDBG_LEAK_CHECK_DF: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an error 
	//report if the application failed to free all the memory it allocated. OFF: Do not automatically perform leak checking at program exit.
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//the following statement is used to trigger a breakpoint when memory leak happens
	//comment it out if there is no memory leak report;
	//_crtBreakAlloc = 16897;
#endif
}

int main()
{
	installMemoryLeakDetector();
	try
	{
		hiveCommon::hiveLoadTestingDLLs();

		std::vector<glm::vec2> TotalPositionSet;
		for (int i=0; i<10; ++i)
		{
			TotalPositionSet.push_back(glm::vec2(i, i));
		}
		TotalPositionSet.push_back(glm::vec2(0.5, 1.0));
		TotalPositionSet.push_back(glm::vec2(1.0, 0.5));
		TotalPositionSet.push_back(glm::vec2(1.0, 1.5));
		TotalPositionSet.push_back(glm::vec2(1.5, 1.0));

		std::vector<glm::vec2> NeighborSet;
		std::vector<unsigned int> NeighborIndexSet;

		hiveCrowdSimulation::CKNNSearch* pNeighborSearch = new hiveCrowdSimulation::CKNNSearch();
		pNeighborSearch->initKNNSearch(TotalPositionSet, 2, 4);
		pNeighborSearch->executeKNN(glm::vec2(1.0, 1.0), NeighborIndexSet);

		for (unsigned int i=0; i<NeighborIndexSet.size(); ++i)
		{
			float Dist = glm::distance(glm::vec2(1.0, 1.0), TotalPositionSet[NeighborIndexSet[i]]);
			_ASSERT(Dist == 0.5f);
			std::cout << TotalPositionSet[NeighborIndexSet[i]].x << " " << TotalPositionSet[NeighborIndexSet[i]].y << std::endl;
		}
		delete pNeighborSearch;
	}
	catch (...)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The program is terminated due to unexpected error.");
	}

	return 0;
}