#include "stdafx.h"
#include "CppUnitTest.h"
#include "MathHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_001
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Test_EigenVector)
		{
			// TODO: 在此输入测试代码
			Eigen::MatrixXd TestMatrix(2, 2);
			TestMatrix <<  3, -1,
				          -1, 3;

			Eigen::VectorXd EigenVector = computeEigenVector(TestMatrix);
			float Test1 = (float)EigenVector.x();
			Assert::AreEqual((float)EigenVector.x(), (float)0.707107,  (float)0.000001);
			Assert::AreEqual((float)EigenVector.y(), (float)-0.707107, (float)0.000001);
		}
	};
}