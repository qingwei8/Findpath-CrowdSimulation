#include "UniformGridScene.h"
#include <set>
#include <iostream>
#include "SceneParser.h"
#include "BaseScene.h"

using namespace hiveSceneProcess;

CUniformGridScene::CUniformGridScene()
{
	m_SizeOfWidth = 0;
	m_SizeOfHeight = 0;
}

CUniformGridScene::~CUniformGridScene()
{
	for (auto& GridVector : m_SceneGridSet)
	{
		for (auto& Grid : GridVector)
		{
			delete Grid;
		}
	}
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CUniformGridScene::dumpPassableSetInsightV(const glm::vec2& vGridCenter, float vDistance, std::vector<const SRectShape*>& voGirdSet) const
{
	if (vDistance <= m_ParameterA / 2 && vDistance <= m_ParameterB / 2)
	{
		return false;
	}
	int PosX = vGridCenter.y / m_ParameterA;
	int PosY = vGridCenter.x / m_ParameterB;
	int DisplacementInX = vDistance / m_ParameterA;
	int DisplacementInY = vDistance / m_ParameterB;

	for (int i=-DisplacementInX; i<=DisplacementInX; ++i)
	{
		for (int k=-DisplacementInY; k<=DisplacementInY; ++k)
		{
			if (abs(i) == DisplacementInX || abs(k) == DisplacementInY)
			{
				CSceneGrid* pGrid = m_SceneGridSet[PosX+i][PosY+k];
				if (pGrid->isPassable())
				{
					voGirdSet.push_back(m_SceneGridSet[PosX+i][PosY+k]->getRect());
				}
			}
		}
	}

	return (voGirdSet.size() != 0);
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CUniformGridScene::dumpAdjacentPassableSetV(const glm::vec2& vGridCenter, std::vector<const SRectShape*>& voAdjSet) const 
{
	glm::uvec2 GridCoord = __getSceneGridCoord(vGridCenter);
	int PosX = GridCoord.x;
	int PosY = GridCoord.y;

	for (int i=-1; i<=1; ++i)
	{
		for (int k=-1; k<=1; ++k)
		{
			if (i != 0 || k != 0)
			{
				if (PosX+i >= 0 && PosX+i <m_SceneGridSet.size() &&  PosY+k >= 0 && PosY+k < m_SceneGridSet[0].size())
				{
					CSceneGrid* pGrid = m_SceneGridSet[PosX+i][PosY+k];
					if (pGrid->isPassable())
					{
						voAdjSet.push_back(m_SceneGridSet[PosX+i][PosY+k]->getRect());
					}
				}
			}
		}
	}
}

//********************************************************************
//FUNCTION:
CSceneGrid* hiveSceneProcess::CUniformGridScene::getSceneGridV(const glm::vec2& vPosInScene) const 
{
	glm::uvec2 GridCoord = __getSceneGridCoord(vPosInScene);
	return m_SceneGridSet[GridCoord.x][GridCoord.y];
}

//********************************************************************
//FUNCTION:
glm::uvec2 hiveSceneProcess::CUniformGridScene::__getSceneGridCoord(const glm::vec2& vScenePos) const
{
	//Todo: need delete
	if ((vScenePos.x > m_SceneWidthInPixel) || (vScenePos.y > m_SceneHeightInPixel))
	{
		std::cout << vScenePos.x << " " << vScenePos.y << std::endl;
	}

	_ASSERT(isPositionInScene(vScenePos));
	int GridPosX = vScenePos.y / m_ParameterA;
	int GridPosY = vScenePos.x / m_ParameterB;
	return glm::uvec2(GridPosX, GridPosY);
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveSceneProcess::CUniformGridScene::__getArrayCoord(const glm::vec2& vScenePos) const
{
	return glm::vec2(floor(vScenePos.x / m_ParameterA), floor(vScenePos.y / m_ParameterB));
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveSceneProcess::CUniformGridScene::getSceneGridCenterV(const glm::vec2& vPosInScene) const 
{
	CSceneGrid* pSceneGrid = getSceneGridV(vPosInScene);
	const SRectShape* pGridRect = pSceneGrid->getRect();
	return pGridRect->getCenter();
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CUniformGridScene::isDirectVisibleV(const glm::vec2& vScenePosA, const glm::vec2& vScenePosB) const 
{
	_ASSERT(!isPosInObstacle(vScenePosA) && !isPosInObstacle(vScenePosB));
	CSceneGrid* pGridA = getSceneGridV(vScenePosA);
	CSceneGrid* pGridB = getSceneGridV(vScenePosB);
	
	if (pGridA->isPassable() == false || pGridB->isPassable() == false)
		return false;

	std::vector<glm::vec2> GridCenterPos;
	computeGridCoordBetweenTwoGrids(vScenePosA, vScenePosB, GridCenterPos);

	for (auto& Grid : GridCenterPos)
	{
		CSceneGrid* pGrid = getSceneGridV(Grid);
		if (!pGrid->isPassable()) return false;
	}

	return true;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CUniformGridScene::testFunction()
{
	for (unsigned int i=0; i<m_SceneGridSet.size(); ++i)
	{
		for (unsigned int k=0; k<m_SceneGridSet[i].size(); ++k)
		{
			const SRectShape* pRect = m_SceneGridSet[i][k]->getRect();
			std::cout << pRect->m_LeftUp.x << " " << pRect->m_LeftUp.y << " " << pRect->m_RightDown.x << " " << pRect->m_RightDown.y << std::endl;
		}
	}
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CUniformGridScene::_generateSceneV(const char* vImageData)
{
	_ASSERT(m_IsDivideSceneInfoSpecified);

	int MinWidth = m_ParameterA;
	int MinHeight = m_ParameterB;
	int HalfCountOfPixel = MinHeight * MinWidth / 2;
	
	int SizeOfWidth = std::ceil(m_SceneWidthInPixel / MinWidth);
	int SizeOfHeight = std::ceil(m_SceneHeightInPixel / MinHeight);

	m_SizeOfWidth = SizeOfWidth;
	m_SizeOfHeight = SizeOfHeight;

	m_SceneGridSet.resize(SizeOfHeight);
	for (int l=0; l<m_SceneGridSet.size(); ++l)
	{
		m_SceneGridSet[l].resize(SizeOfWidth);
	}

	int i = 0;
	int k;
	while (i < SizeOfHeight)
	{
		k = 0;
		while (k < SizeOfWidth)
		{
			int Count = 0;
			int m, n;
			for (m=0; m<MinHeight && (i*MinHeight+m)<m_SceneHeightInPixel; ++m)
			{
				for (n=0; n<MinWidth && (k*MinWidth+n)<m_SceneWidthInPixel; ++n)
				{
					if (vImageData[(i * MinHeight + m) * m_SceneWidthInPixel + k * MinWidth + n] == 0)
					{
						Count++;
					}
				}
			}
			bool Passable = Count > HalfCountOfPixel ? false : true;
			SRectShape* pRect = new SRectShape(glm::vec2(k*MinWidth, i*MinHeight), glm::vec2(k*MinWidth+m, i*MinHeight+n));
			CSceneGrid* pTemp = new CSceneGrid(pRect, Passable);
			m_SceneGridSet[i][k] = pTemp;
			Passable ? _addPassableGrid(pTemp) : _addObstacleGrid(pTemp);
			k++;
		}
		i++;
	}

	delete vImageData;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CUniformGridScene::computeGridCoordBetweenTwoGrids(const glm::vec2& vScenePosA, const glm::vec2& vScenePosB, std::vector<glm::vec2>& voScenePosResult) const
{
	glm::vec2 ArrayCoordA = __getArrayCoord(vScenePosA);
	glm::vec2 ArrayCoordB = __getArrayCoord(vScenePosB);
	std::vector<glm::vec2> ArrayCoordResult;
	if (ArrayCoordA.x == ArrayCoordB.x)//Todo:x坐标相同，同一竖线上
	{
		__handleXArrayCoordEqual(ArrayCoordA, ArrayCoordB, ArrayCoordResult);
	}
	else if (ArrayCoordA.y == ArrayCoordB.y)//y坐标相同，同一横线上
	{
		__handleYArrayCoordEqual(ArrayCoordA, ArrayCoordB, ArrayCoordResult);
	}
	else
	{
		float DifferenceX = ArrayCoordA.x - ArrayCoordB.x;
		float DifferenceY = ArrayCoordA.y - ArrayCoordB.y;
		if (DifferenceX == DifferenceY)
		{
			__handleEqualSlope(ArrayCoordA, ArrayCoordB, ArrayCoordResult);//xy增长率相同，比如（2， 6），（5， 9）
		}
		else if (DifferenceX == -DifferenceY)
		{
			__handleReverseSlope(ArrayCoordA, ArrayCoordB, ArrayCoordResult);//xy增长率相反，比如（2， 6），（5， 3）
		}
		else
		{
			__handleOtherCases(vScenePosA, vScenePosB, ArrayCoordResult);//其他
		}
	}

	for (unsigned int i=0; i<ArrayCoordResult.size(); ++i)
	{
		glm::vec2 VectorCoord = glm::vec2(ArrayCoordResult[i].y, ArrayCoordResult[i].x);
		CSceneGrid* pGrid     = m_SceneGridSet[VectorCoord.x][VectorCoord.y];
		voScenePosResult.push_back(pGrid->getRect()->getCenter());
	}
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CUniformGridScene::__handleOtherCases(const glm::vec2& vScenePosA, const glm::vec2& vScenePosB, std::vector<glm::vec2>& voArrayCoordResult) const
{
	//将opencv的坐标(x指向右，y指向下)转换成一般坐标(x指向右，y指向上)

	glm::vec2 NormalCoordPosA = glm::vec2(vScenePosA.x, m_SceneHeightInPixel - vScenePosA.y);
	glm::vec2 NormalCoordPosB = glm::vec2(vScenePosB.x, m_SceneHeightInPixel - vScenePosB.y);
	SStraightLine Line = SStraightLine(NormalCoordPosA, NormalCoordPosB);

	float ArrayCoordYMin, ArrayCoordYMax;
	glm::vec2 ArrayCoordA = glm::vec2(floor(NormalCoordPosA.x / m_ParameterA), floor(NormalCoordPosA.y / m_ParameterB));
	glm::vec2 ArrayCoordB = glm::vec2(floor(NormalCoordPosB.x / m_ParameterA), floor(NormalCoordPosB.y / m_ParameterB));
	compareTwoNumber(ArrayCoordA.y, ArrayCoordB.y, ArrayCoordYMax, ArrayCoordYMin);

	std::set<glm::vec2, SCompareFunction> ResultSet;
	//Todo: 计算line与横线的交点，取交点的上下两个格子。
	int NumLines = m_SceneHeightInPixel / m_ParameterB;
	for (int TempArrayY=ArrayCoordYMin+1; TempArrayY<=ArrayCoordYMax; ++TempArrayY)
	{
		float TempScenePosY = TempArrayY * m_ParameterB;
		float TempScenePosX = Line.computeCorrespondingX(TempScenePosY);
		float ArrayCoordX = floor(TempScenePosX / m_ParameterA);

		ResultSet.insert(glm::vec2(ArrayCoordX, (NumLines - 1) - (TempArrayY - 1)));//Todo:世界坐标转回opencv坐标
		ResultSet.insert(glm::vec2(ArrayCoordX, (NumLines - 1) - TempArrayY));
	}

	float ArrayCoordXMin, ArrayCoordXMax;
	compareTwoNumber(ArrayCoordA.x, ArrayCoordB.x, ArrayCoordXMax, ArrayCoordXMin);
	for (int TempArrayX=ArrayCoordXMin+1; TempArrayX<=ArrayCoordXMax; ++TempArrayX)
	{
		float TempScenePosX = TempArrayX * m_ParameterA;
		float TempScenePosY = Line.computeCorrespondingY(TempScenePosX);
		float ArrayCoordY = floor(TempScenePosY / m_ParameterB);

		ResultSet.insert(glm::vec2(TempArrayX - 1, (NumLines - 1) - ArrayCoordY));
		ResultSet.insert(glm::vec2(TempArrayX,     (NumLines - 1) - ArrayCoordY));
	}

	voArrayCoordResult.resize(ResultSet.size());
	std::copy(ResultSet.begin(), ResultSet.end(), voArrayCoordResult.begin());
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CUniformGridScene::__handleXArrayCoordEqual(const glm::vec2& vArrayCoordA, const glm::vec2& vArrayCoordB, std::vector<glm::vec2>& voArrayCoordResult) const
{
	_ASSERT(vArrayCoordA.x == vArrayCoordB.x);
	float MinArrayCoordY, MaxArrayCoordY;
	compareTwoNumber(vArrayCoordA.y, vArrayCoordB.y, MaxArrayCoordY, MinArrayCoordY);

	for (int i=MinArrayCoordY+1; i<MaxArrayCoordY; ++i)
	{
		voArrayCoordResult.push_back(glm::vec2(vArrayCoordA.x, i));
	}
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CUniformGridScene::__handleYArrayCoordEqual(const glm::vec2& vArrayCoordA, const glm::vec2& vArrayCoordB, std::vector<glm::vec2>& voArrayCoordResult) const
{
	_ASSERT(vArrayCoordA.y == vArrayCoordB.y);
	float MinArrayCoordX, MaxArrayCoordX;
	compareTwoNumber(vArrayCoordA.x, vArrayCoordB.x, MaxArrayCoordX, MinArrayCoordX);

	for (int i=MinArrayCoordX+1; i<MaxArrayCoordX; ++i)
	{
		voArrayCoordResult.push_back(glm::vec2(i, vArrayCoordA.y));
	}
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CUniformGridScene::__handleEqualSlope(const glm::vec2& vArrayCoordA, const glm::vec2& vArrayCoordB, std::vector<glm::vec2>& voArrayCoordResult) const
{
	float DifferenceX = vArrayCoordA.x - vArrayCoordB.x;
	float DifferenceY = vArrayCoordA.y - vArrayCoordB.y;
	_ASSERT(DifferenceX == DifferenceY);

	float MinX, MaxX, MinY, MaxY;
	compareTwoNumber(vArrayCoordA.x, vArrayCoordB.x, MaxX, MinX);
	compareTwoNumber(vArrayCoordA.y, vArrayCoordB.y, MaxY, MinY);

	int StartX = MinX + 1;
	int StartY = MinY + 1;
	while (StartX < MaxX && StartY < MaxY)
	{
		voArrayCoordResult.push_back(glm::vec2(StartX, StartY));
		StartX++;
		StartY++;
	}
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CUniformGridScene::__handleReverseSlope(const glm::vec2& vArrayCoordA, const glm::vec2& vArrayCoordB, std::vector<glm::vec2>& voArrayCoordResult) const
{
	float DifferenceX = vArrayCoordA.x - vArrayCoordB.x;
	float DifferenceY = vArrayCoordA.y - vArrayCoordB.y;
	_ASSERT(DifferenceX == -DifferenceY);

	float MinX, MaxX, MinY, MaxY;
	compareTwoNumber(vArrayCoordA.x, vArrayCoordB.x, MaxX, MinX);
	compareTwoNumber(vArrayCoordA.y, vArrayCoordB.y, MaxY, MinY);

	int StartX = MinX + 1;
	int StartY = MaxY - 1;
	while (StartX < MaxX && StartY > MinY)
	{
		voArrayCoordResult.push_back(glm::vec2(StartX, StartY));
		StartX++;
		StartY--;
	}
}

//********************************************************************
//FUNCTION:	
bool hiveSceneProcess::CUniformGridScene::__isSegmentAndRectangleIntersect(const SStraightLine& vLine, const SRectShape* vNode) const
{
	int NodeMinX = vNode->m_LeftUp.x;
	int NodeMaxX = vNode->m_RightDown.x;
	int NodeMinY = vNode->m_LeftUp.y;
	int NodeMaxY = vNode->m_RightDown.y;
	float Y1 = vLine.computeCorrespondingY(NodeMinX);
	float Y2 = vLine.computeCorrespondingY(NodeMaxX);
	float X1 = vLine.computeCorrespondingX(NodeMinY);
	float X2 = vLine.computeCorrespondingX(NodeMaxY);
	float LineMaxX = vLine.getMaxX();
	float LineMaxY = vLine.getMaxY();
	float LineMinX = vLine.getMinX();
	float LineMinY = vLine.getMinY();

	int CountOfIntersection = 0;
	if (Y1 == -1 && vLine.PointA.x >= NodeMinX && vLine.PointA.x < NodeMaxX)
		CountOfIntersection++;
	if (X1 == -1 && vLine.PointA.y >= NodeMinY && vLine.PointA.y < NodeMaxY)
		CountOfIntersection++;
	if (Y1 >= NodeMinY && Y1 <= NodeMaxY)
		CountOfIntersection++;
	if (Y2 >= NodeMinY && Y2 <= NodeMaxY)
		CountOfIntersection++;
	if (X1 >= NodeMinX && X1 <= NodeMaxX)
		CountOfIntersection++;
	if (X2 >= NodeMinX && X2 <= NodeMaxX)
		CountOfIntersection++;

	if (vLine.getSlope() < 0)
	{
		if (X1 == NodeMinX && Y1 == NodeMinY)
			return false;
		if (X2 == NodeMaxX && Y2 == NodeMaxY)
			return false;
	}
	if (vLine.getSlope() > 0)
	{
		if (X1 == NodeMinX && Y2 == NodeMaxY)
			return false;
		if (X2 == NodeMaxX && Y1 == NodeMinY)
			return false;
	}

	if (CountOfIntersection >= 2)
		return (NodeMinX <= LineMaxX && NodeMinY <= LineMaxY && NodeMaxX >= LineMinX && NodeMaxY >= LineMinY);
	else
		return false;
}