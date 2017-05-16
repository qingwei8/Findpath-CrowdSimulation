#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CrowdSimulation/KNNSearch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_002KNN
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Test_KNN)
		{
			// TODO: 在此输入测试代码
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
				Assert::AreEqual(Dist, 0.5f);
			}
		}
	};
}