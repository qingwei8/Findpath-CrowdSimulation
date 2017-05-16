#include "stdafx.h"
#include "CppUnitTest.h"
#include "UniformGridScene.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_003两点经过的网格
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Test_可见性)
		{
			// TODO: 在此输入测试代码
			int Width  = 32;
			int Height = 32;
			std::string SceneImage = "../TestCases/ScenePic/Scene_512_512_0.bmp";
			hiveSceneProcess::CUniformGridScene* pUniformGridScene = new hiveSceneProcess::CUniformGridScene();
			pUniformGridScene->setParameter(Width, Height);
			pUniformGridScene->generateScene(SceneImage);

			//Todo: 测试1：同一竖直直线
			glm::vec2 ScenePosA1 = glm::vec2(48, 176);/*glm::vec2(1, 5);*/
			glm::vec2 ScenePosB1 = glm::vec2(48, 336);/*glm::vec2(1, 10);*/
			std::vector<glm::vec2> GridCenterRes1;
			pUniformGridScene->computeGridCoordBetweenTwoGrids(ScenePosA1, ScenePosB1, GridCenterRes1);

			Assert::AreEqual(GridCenterRes1[0].x, 48.0f);
			Assert::AreEqual(GridCenterRes1[0].y, 208.0f);
			Assert::AreEqual(GridCenterRes1[1].x, 48.0f);
			Assert::AreEqual(GridCenterRes1[1].y, 240.0f);
			Assert::AreEqual(GridCenterRes1[2].x, 48.0f);
			Assert::AreEqual(GridCenterRes1[2].y, 272.0f);
			Assert::AreEqual(GridCenterRes1[3].x, 48.0f);
			Assert::AreEqual(GridCenterRes1[3].y, 304.0f);

			//Todo:测试2：同一横水平直线
 			glm::vec2 ScenePosA2 = glm::vec2(48, 176);/*glm::vec2(1, 5);*/
 			glm::vec2 ScenePosB2 = glm::vec2(336, 176);/*glm::vec2(10, 5);*/
 			std::vector<glm::vec2> GridCenterRes2;
 			pUniformGridScene->computeGridCoordBetweenTwoGrids(ScenePosA2, ScenePosB2, GridCenterRes2);
			
			Assert::AreEqual((int)GridCenterRes2.size(), 8);
			Assert::AreEqual(GridCenterRes2[0].x, 80.0f);
			Assert::AreEqual(GridCenterRes2[0].y, 176.0f);
			Assert::AreEqual(GridCenterRes2[1].x, 112.0f);
			Assert::AreEqual(GridCenterRes2[1].y, 176.0f);
			Assert::AreEqual(GridCenterRes2[2].x, 144.0f);
			Assert::AreEqual(GridCenterRes2[2].y, 176.0f);
			Assert::AreEqual(GridCenterRes2[3].x, 176.0f);
			Assert::AreEqual(GridCenterRes2[3].y, 176.0f);
			Assert::AreEqual(GridCenterRes2[4].x, 208.0f);
			Assert::AreEqual(GridCenterRes2[4].y, 176.0f);
			Assert::AreEqual(GridCenterRes2[5].x, 240.0f);
			Assert::AreEqual(GridCenterRes2[5].y, 176.0f);
			Assert::AreEqual(GridCenterRes2[6].x, 272.0f);
			Assert::AreEqual(GridCenterRes2[6].y, 176.0f);
			Assert::AreEqual(GridCenterRes2[7].x, 304.0f);
			Assert::AreEqual(GridCenterRes2[7].y, 176.0f);

			//Todo:测试3：斜率为正
			glm::vec2 ScenePosA3 = glm::vec2(38, 70);/*glm::vec2(1, 2);*/
			glm::vec2 ScenePosB3 = glm::vec2(170, 202);/*glm::vec2(5, 6);*/
			std::vector<glm::vec2> GridCenterRes3;
			pUniformGridScene->computeGridCoordBetweenTwoGrids(ScenePosA3, ScenePosB3, GridCenterRes3);

			Assert::AreEqual((int)GridCenterRes3.size(), 3);
			Assert::AreEqual(GridCenterRes3[0].x, 80.0f);
			Assert::AreEqual(GridCenterRes3[0].y, 112.0f);
			Assert::AreEqual(GridCenterRes3[1].x, 112.0f);
			Assert::AreEqual(GridCenterRes3[1].y, 144.0f);
			Assert::AreEqual(GridCenterRes3[2].x, 144.0f);
			Assert::AreEqual(GridCenterRes3[2].y, 176.0f);

			//Todo:测试4：斜率为负
			glm::vec2 ScenePosA4 = glm::vec2(40, 180);/*glm::vec2(1, 5);*/
			glm::vec2 ScenePosB4 = glm::vec2(182, 42);/*glm::vec2(5, 1);*/
			std::vector<glm::vec2> GridCenterRes4;
			pUniformGridScene->computeGridCoordBetweenTwoGrids(ScenePosA4, ScenePosB4, GridCenterRes4);

			Assert::AreEqual((int)GridCenterRes4.size(), 3);
			Assert::AreEqual(GridCenterRes4[0].x, 80.0f);
			Assert::AreEqual(GridCenterRes4[0].y, 144.0f);
			Assert::AreEqual(GridCenterRes4[1].x, 112.0f);
			Assert::AreEqual(GridCenterRes4[1].y, 112.0f);
			Assert::AreEqual(GridCenterRes4[2].x, 144.0f);
			Assert::AreEqual(GridCenterRes4[2].y, 80.0f);

			//Todo：测试5：其他情况测试
			glm::vec2 ScenePosA5 = glm::vec2(16, 16);//(0, 6)
			glm::vec2 ScenePosB5 = glm::vec2(144, 112);//(4, 0)
			std::vector<glm::vec2> GridCenterRes5;
			pUniformGridScene->computeGridCoordBetweenTwoGrids(ScenePosA5, ScenePosB5, GridCenterRes5);

			Assert::AreEqual((int)GridCenterRes5.size(), 8);
			Assert::AreEqual(GridCenterRes5[0].x, 16.0f);
			Assert::AreEqual(GridCenterRes5[0].y, 16.0f);

			Assert::AreEqual(GridCenterRes5[1].x, 48.0f);
			Assert::AreEqual(GridCenterRes5[1].y, 16.0f);

			Assert::AreEqual(GridCenterRes5[2].x, 48.0f);
			Assert::AreEqual(GridCenterRes5[2].y, 48.0f);

			Assert::AreEqual(GridCenterRes5[3].x, 80.0f);
			Assert::AreEqual(GridCenterRes5[3].y, 48.0f);

			Assert::AreEqual(GridCenterRes5[4].x, 80.0f);
			Assert::AreEqual(GridCenterRes5[4].y, 80.0f);

			Assert::AreEqual(GridCenterRes5[5].x, 112.0f);
			Assert::AreEqual(GridCenterRes5[5].y, 80.0f);

			Assert::AreEqual(GridCenterRes5[6].x, 112.0f);
			Assert::AreEqual(GridCenterRes5[6].y, 112.0f);

			Assert::AreEqual(GridCenterRes5[7].x, 144.0f);
			Assert::AreEqual(GridCenterRes5[7].y, 112.0f);

			//Todo: 测试可见性
			glm::vec2 ScenePosSrc = glm::vec2(200, 100);
			glm::vec2 ScenePosDst = glm::vec2(200, 480);
			bool Visible = pUniformGridScene->isDirectVisibleV(ScenePosSrc, ScenePosDst);
			Assert::IsFalse(Visible);
		}
	};
}