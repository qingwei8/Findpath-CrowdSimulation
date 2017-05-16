#include "KNNSearch.h"

using namespace hiveCrowdSimulation;

CKNNSearch::CKNNSearch(void) : m_IsKDTreeBuilt(false), m_NumPoint(0), m_NumNeighbor(0), m_Dimension(0), m_LeafMaxSize(10), m_pReorderData(NULL), m_pRootNode(NULL), m_pInputData(NULL)
{
}

CKNNSearch::~CKNNSearch(void)
{
	__destructData();
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CKNNSearch::__destructData()
{
	if (m_pReorderData != NULL)
	{
		delete[] m_pReorderData;
		m_pReorderData = NULL;
	}
	
	if (m_pInputData != NULL)
	{
		delete[] m_pInputData;
		m_pInputData = NULL;
	}
	
	__deleteTree();
}

//*********************************************************************************
//FUNCTION: 
void CKNNSearch::initKNNSearch(const std::vector<glm::vec2>& vInputPoints, unsigned int vDimension, unsigned int vNumNeighbor)
{	
	_ASSERT(vInputPoints.size() && vNumNeighbor > 0 && vDimension > 0 && vNumNeighbor <= vInputPoints.size());

	unsigned int NumInputPoint = vInputPoints.size();
	float* pInputPoints = new float[NumInputPoint*2];
	for (size_t i=0; i<NumInputPoint; ++i)
	{
		pInputPoints[i*2+0] = vInputPoints[i].x;
		pInputPoints[i*2+1] = vInputPoints[i].y;
	}

	m_NumPoint    = NumInputPoint; 	
	m_NumNeighbor = vNumNeighbor + 1;
	m_Dimension   = vDimension;
	m_pReorderData = NULL;
	m_pRootNode   = NULL;
	m_IndexOfData.clear();
	m_IndexOfData.resize(m_NumPoint, 0);
	for (unsigned int i=0; i<m_NumPoint; ++i)
		m_IndexOfData[i] = i;

	m_pInputData = pInputPoints;
	__buildSearchIndex();
	m_IsKDTreeBuilt = true;
}

//*********************************************************************************
//FUNCTION
void CKNNSearch::executeKNN(const glm::vec2& vTargetPoint, std::vector<unsigned int>& voKNNResultSet)
{
	_ASSERT(m_IsKDTreeBuilt);

	voKNNResultSet.resize(m_NumNeighbor);
	std::vector<float> Distances;
	Distances.resize(m_NumNeighbor, 0.0);
	std::vector<float> Query (m_Dimension);
	for (unsigned int i=0; i<m_Dimension; ++i)
		Query[i] = vTargetPoint[i];

	__searchKNearestNeighbor(&Query[0], &voKNNResultSet[0], &Distances[0]);

	voKNNResultSet.erase(voKNNResultSet.begin());
}

//*********************************************************************************
//FUNCTION
void CKNNSearch::__buildSearchIndex()
{
	__computeBoundingBox(m_RootBbox);
	m_pRootNode = __divideTree(0, m_NumPoint, m_RootBbox);

	m_pReorderData = new float[m_NumPoint*m_Dimension];
	for (unsigned int i=0; i<m_NumPoint; ++i) 
	{
		for (unsigned int k=0; k<m_Dimension; ++k) 
		{
			m_pReorderData[i*m_Dimension+k] = m_pInputData[m_IndexOfData[i]*m_Dimension+k];
		}
	}
}

//*********************************************************************************
//FUNCTION
void CKNNSearch::__computeBoundingBox(AABBBoundingBox& voBBox)
{
	voBBox.resize(m_Dimension);
	for (unsigned int i=0; i<m_Dimension; ++i) 
	{
		voBBox[i].Low  = m_pInputData[i];
		voBBox[i].High = m_pInputData[i];
	}

	for (unsigned int i=1; i<m_NumPoint; ++i) 
	{ 
		for (unsigned int k=0; k<m_Dimension; ++k) 
		{
			int Index = i * m_Dimension + k;

			if (m_pInputData[Index] < voBBox[k].Low)
				voBBox[k].Low = m_pInputData[Index];
			if (m_pInputData[Index] > voBBox[k].High)
				voBBox[k].High = m_pInputData[Index];
		}
	}
}

//*********************************************************************************
//FUNCTION
void CKNNSearch::__computeMinMax(const int *vIndices, int vNumInputPoint, int vSplitAxis, float& voMinElem, float& voMaxElem)
{
	int Index = vIndices[0] * m_Dimension + vSplitAxis;

	voMinElem = m_pInputData[Index];
	voMaxElem = m_pInputData[Index];
	for (int i=1; i<vNumInputPoint; ++i) 
	{
		float TempValue = m_pInputData[vIndices[i]*m_Dimension+vSplitAxis];
		if (TempValue < voMinElem) voMinElem = TempValue;
		if (TempValue > voMaxElem) voMaxElem = TempValue;
	}
}

//*********************************************************************************
//FUNCTION
void CKNNSearch::__splitPlane(int vNumInputPoint, int vSplitAxis, float vSplitValue, int *vioIndices, int& voIndexLTSplitValue, int& voIndexEorLTSplitValue)
{
	int Left  = 0;
	int Right = vNumInputPoint - 1;
	while (true)
	{	
		while (Left<=Right && m_pInputData[vioIndices[Left] *m_Dimension+vSplitAxis]< vSplitValue)  ++Left;
		while (Left<=Right && m_pInputData[vioIndices[Right]*m_Dimension+vSplitAxis]>=vSplitValue) --Right;
		if (Left > Right) break;
		std::swap(vioIndices[Left], vioIndices[Right]); ++Left; --Right;
	}

	voIndexLTSplitValue = Left;
	Right = vNumInputPoint - 1;
	while (true)
	{
		while (Left<=Right && m_pInputData[vioIndices[Left] *m_Dimension+vSplitAxis]<=vSplitValue) ++Left;
		while (Left<=Right && m_pInputData[vioIndices[Right]*m_Dimension+vSplitAxis]> vSplitValue)  --Right;
		if (Left > Right) break;
		std::swap(vioIndices[Left], vioIndices[Right]); ++Left; --Right;
	}
	voIndexEorLTSplitValue = Left;
}

//*********************************************************************************
//FUNCTION
void CKNNSearch::__split(const AABBBoundingBox& vBBox, int *vioIndices, int vNumInputPoint, int& voIndexOffset, int& voSplitAxis, float& voSplitValue)
{
	float MaxSpan = vBBox[0].High - vBBox[0].Low;
	voSplitAxis    = 0;
	voSplitValue   = (vBBox[0].High + vBBox[0].Low) / 2;
	for (unsigned int i=1; i<m_Dimension; ++i)
	{
		float Span = vBBox[i].High - vBBox[i].Low;
		if (Span > MaxSpan) 
		{
			MaxSpan      = Span;
			voSplitAxis  = i;
			voSplitValue = (vBBox[i].High+vBBox[i].Low)/2;
		}
	}

	float MinElem = 0.0, MaxElem = 0.0;
	__computeMinMax(vioIndices, vNumInputPoint, voSplitAxis, MinElem, MaxElem);
	voSplitValue = (MinElem + MaxElem) / 2;
	MaxSpan = MaxElem - MinElem;

	unsigned int k = voSplitAxis;
	for (unsigned int i=0; i<m_Dimension; ++i) 
	{
		if (i == k) continue;
		float Span = vBBox[i].High - vBBox[i].Low;
		if (Span > MaxSpan) 
		{
			__computeMinMax(vioIndices, vNumInputPoint, i, MinElem, MaxElem);
			Span = MaxElem - MinElem;
			if (Span > MaxSpan) 
			{
				MaxSpan      = Span;
				voSplitAxis  = i;
				voSplitValue = (MinElem + MaxElem) / 2;
			}
		}
	}

	int Index1, Index2;
	__splitPlane(vNumInputPoint, voSplitAxis, voSplitValue, vioIndices, Index1, Index2);

	if (Index1 > vNumInputPoint/2) voIndexOffset = Index1;
	else if (Index2 < vNumInputPoint/2) voIndexOffset = Index2;
	else voIndexOffset = vNumInputPoint / 2;
}

//*********************************************************************************
//FUNCTION
CKNNSearch::SNode* CKNNSearch::__divideTree(int vLeft, int vRight, AABBBoundingBox& voBBox)
{
	SNode* pNode = new SNode;

	if ((vRight-vLeft) <= m_LeafMaxSize) 
	{
		pNode->pLeftChild = pNode->pRightChild = NULL;
		pNode->LeftIndex  = vLeft;
		pNode->RightIndex = vRight;

		for (unsigned int i=0; i<m_Dimension; ++i) 
		{
			int Index = m_IndexOfData[vLeft] * m_Dimension + i;

			voBBox[i].Low  = m_pInputData[Index];
			voBBox[i].High = m_pInputData[Index];
		}
		for (int k=vLeft+1; k<vRight; ++k) 
		{
			for (unsigned int i=0; i<m_Dimension; ++i) 
			{
				int Index = m_IndexOfData[k] * m_Dimension + i;

				if (voBBox[i].Low  > m_pInputData[Index])
					voBBox[i].Low  = m_pInputData[Index];
				if (voBBox[i].High < m_pInputData[Index])
					voBBox[i].High = m_pInputData[Index];
			}
		}
	}
	else 
	{
		int SplitAxis;
		int IndexOffset;
		float SplitValue;
		__split(voBBox, &m_IndexOfData[0]+vLeft, vRight-vLeft, IndexOffset, SplitAxis, SplitValue);

		pNode->SplitAxis = SplitAxis;

		AABBBoundingBox LeftBBox(voBBox);
		LeftBBox[SplitAxis].High = SplitValue;
		pNode->pLeftChild = __divideTree(vLeft, vLeft+IndexOffset, LeftBBox);

		AABBBoundingBox RightBBox(voBBox);
		RightBBox[SplitAxis].Low = SplitValue;
		pNode->pRightChild = __divideTree(vLeft+IndexOffset, vRight, RightBBox);

		pNode->Splitlow  = LeftBBox[SplitAxis].High;
		pNode->Splithigh = RightBBox[SplitAxis].Low;

		for (unsigned int i=0; i<m_Dimension; ++i) 
		{
			voBBox[i].Low  = std::min(LeftBBox[i].Low,  RightBBox[i].Low);
			voBBox[i].High = std::max(LeftBBox[i].High, RightBBox[i].High);
		}
	}
	return pNode;
}


//*********************************************************************************
//FUNCTION
void CKNNSearch::__searchKNearestNeighbor(const float *vQueries, unsigned int *voKIdices, float *voKDistance)
{
	CKNNSimpleResultSet<float> ResultSet(m_NumNeighbor);
	ResultSet.clear();
	__findNeighbors(vQueries, ResultSet);
	ResultSet.copy(voKIdices, voKDistance, m_NumNeighbor, true);
}

//*********************************************************************************
//FUNCTION
void CKNNSearch::__findNeighbors(const float *vPoint, CKNNSimpleResultSet<float>& voResult)
{
	std::vector<float> Distance(m_Dimension, 0);
	float DistanceSquare = __computeInitialDistances(vPoint, Distance);
	__searchLevel(vPoint, m_pRootNode, DistanceSquare, Distance, voResult);
}

//*********************************************************************************
//FUNCTION
float CKNNSearch::__computeInitialDistances(const float *vPoint, std::vector<float>& voDistance)
{
	float DistSq = 0.0;
	AABBBoundingBox AABBBox = m_RootBbox;
	for (unsigned int i=0; i<m_Dimension; ++i) 
	{
		if (vPoint[i] < AABBBox[i].Low) 
		{
			voDistance[i] = m_Distance.computeDistance(vPoint[i], AABBBox[i].Low);
			DistSq += voDistance[i];
		}
		if (vPoint[i] > AABBBox[i].High) 
		{
			voDistance[i] = m_Distance.computeDistance(vPoint[i], AABBBox[i].High);
			DistSq += voDistance[i];
		}
	}
	return DistSq;
}

//*********************************************************************************
//FUNCTION
void CKNNSearch::__searchLevel(const float *vPoint, const SNode *vNode, float voMindistsq, std::vector<float>& vDists, CKNNSimpleResultSet<float>& voResult)
{
	if ((NULL == vNode->pLeftChild) && (NULL == vNode->pRightChild)) 
	{
		float WorstDist = voResult.getWorstDistance();
		for (int i=vNode->LeftIndex; i<vNode->RightIndex; ++i) 
		{
			int Index = i;
			float *TempMatrix  = m_pReorderData;
			float TempDistance = m_Distance(vPoint, &TempMatrix[Index*m_Dimension], m_Dimension);
			if (TempDistance < WorstDist) 
			{
				voResult.addPoint(TempDistance,m_IndexOfData[i]);
			}
		}
		return;
	}

	int Idx = vNode->SplitAxis;
	float TempVal = vPoint[Idx];
	float Diff1   = TempVal - vNode->Splitlow;
	float Diff2   = TempVal - vNode->Splithigh;
	float CutDist = 0.0;
	SNode* pBestChild  = NULL;
	SNode* pOtherChild = NULL;
	if ((Diff1 + Diff2) < 0) 
	{
		pBestChild  = vNode->pLeftChild;
		pOtherChild = vNode->pRightChild;
		CutDist = m_Distance.computeDistance(TempVal, vNode->Splithigh);
	}
	else 
	{
		pBestChild  = vNode->pRightChild;
		pOtherChild = vNode->pLeftChild;
		CutDist = m_Distance.computeDistance(TempVal, vNode->Splitlow);
	}

	__searchLevel(vPoint, pBestChild, voMindistsq, vDists, voResult);

	float TempDis = vDists[Idx];
	voMindistsq  = voMindistsq + CutDist - TempDis;
	vDists[Idx] = CutDist;
	if (voMindistsq <= voResult.getWorstDistance()) 
	{
		__searchLevel(vPoint, pOtherChild, voMindistsq, vDists, voResult);
	}
	vDists[Idx] = TempDis;
}

//*********************************************************************************
//FUNCTION
void CKNNSearch::__deleteTree()
{
	std::stack<bool>    IsRightUsed;
	std::stack<SNode*> TreeNodeStack;
	SNode* pTempNode = m_pRootNode;
	while(pTempNode != NULL)
	{
		while(pTempNode->pLeftChild != NULL || pTempNode->pRightChild != NULL)
		{
			while(pTempNode->pLeftChild != NULL) 
			{
				if (pTempNode->pRightChild != NULL) IsRightUsed.push(false);
				else IsRightUsed.push(true);
				TreeNodeStack.push(pTempNode);
				pTempNode = pTempNode->pLeftChild;
			}
			if (pTempNode->pRightChild != NULL)
			{
				IsRightUsed.push(true);
				TreeNodeStack.push(pTempNode);
				pTempNode = pTempNode->pRightChild;
			}
		}
		delete pTempNode;
		while(!TreeNodeStack.empty())
		{
			if (!IsRightUsed.top())
			{
				pTempNode = TreeNodeStack.top()->pRightChild;
				IsRightUsed.pop();
				IsRightUsed.push(true);
				break;
			}
			else 
			{
				delete TreeNodeStack.top();
				TreeNodeStack.pop();
				IsRightUsed.pop();
			}
		}
		if (TreeNodeStack.empty()) pTempNode = NULL;
	}
	m_pRootNode = NULL;
}