#pragma once
#include <algorithm>
#include <limits>
#include <vector>
#include <stack>
#include <GLM/glm.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/detail/interface_oarchive.hpp>
#include <boost/archive/detail/interface_iarchive.hpp>
#include "SimulationExport.h"

namespace hiveCrowdSimulation
{
	class CROWD_SIMULATION_DLL_EXPORT CKNNSearch
	{
	public:
		CKNNSearch(void);
		virtual ~CKNNSearch(void);

		void initKNNSearch(const std::vector<glm::vec2>& vInputPoints, unsigned int vDimension, unsigned int vNumNeighbor);
		void executeKNN(const glm::vec2& vTargetPoint, std::vector<unsigned int>& voKNNResultSet);

	private:
		struct SNode 
		{
			int LeftIndex, RightIndex;
			int SplitAxis;
			float Splitlow, Splithigh;

			SNode* pLeftChild;
			SNode* pRightChild;

			SNode() : LeftIndex(0), RightIndex(0), SplitAxis(0), Splitlow(0.0), Splithigh(0.0), 
				pLeftChild(NULL), pRightChild(NULL) {}

			friend class boost::serialization::access;

			template<class Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar & LeftIndex;
				ar & RightIndex;
				ar & SplitAxis;
				ar & Splitlow;
				ar & Splithigh;
			}
		};

		struct SInterval
		{
			float Low, High;
			SInterval() : Low(0.0), High(0.0) {}

			friend class boost::serialization::access;

			template<class Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar & Low;
				ar & High;
			}
		};
		typedef std::vector<SInterval> AABBBoundingBox;

		template<class ElementType>
		class CKNNSimpleResultSet 
		{
		public:
			CKNNSimpleResultSet(unsigned int vCapacity) : m_Capacity(vCapacity)
			{
				m_DistIndex.resize(vCapacity, SDistanceIndex<ElementType>(std::numeric_limits<ElementType>::max(),-1));
				clear();
			}

			~CKNNSimpleResultSet(){ }

			void clear()
			{
				m_WorstDis = std::numeric_limits<ElementType>::max();
				m_DistIndex[m_Capacity-1].Distance = m_WorstDis;
				m_Count = 0;
			}

			unsigned int size() const
			{
				return m_Count;
			}

			bool full() const
			{
				return m_Count == m_Capacity;
			}

			void addPoint(ElementType vDist, unsigned int vIndex)
			{
				if (vDist >= m_WorstDis) return;

				if (m_Count < m_Capacity) ++m_Count;
				unsigned int i = 0;
				for (i=m_Count-1; i>0; --i){
					if (m_DistIndex[i-1].Distance>vDist)
					{
						m_DistIndex[i] = m_DistIndex[i-1];
					}
					else break;
				}
				m_DistIndex[i].Distance = vDist;
				m_DistIndex[i].Index = vIndex;
				m_WorstDis = m_DistIndex[m_Capacity-1].Distance;
			}

			void copy(unsigned int *voIndices, ElementType *voDistance, unsigned int vNumOfElement, bool sorted = true)
			{
				unsigned int n = std::min(m_Count, vNumOfElement);
				for (unsigned int i=0; i<n; ++i) {
					*voIndices++ = m_DistIndex[i].Index;
					*voDistance++ = m_DistIndex[i].Distance;
				}
			}

			ElementType getWorstDistance() const
			{
				return m_WorstDis; 
			}

		private:
			template<class ElementType>

			struct SDistanceIndex
			{

				SDistanceIndex(ElementType vDist, unsigned int vIndex) : Distance(vDist), Index(vIndex){}
				ElementType Distance;
				unsigned int Index;
			};
			unsigned int m_Capacity;
			unsigned int m_Count;
			ElementType m_WorstDis;
	
			std::vector<SDistanceIndex<ElementType>> m_DistIndex;
		};

	private:
		void   __buildSearchIndex();
		void   __computeBoundingBox(AABBBoundingBox& voBbox);
		void   __computeMinMax(const int *vIndices, int vNumInputPoint, int vDivAxis, float& voMinElem, float& voMaxElem);
		void   __findNeighbors(const float *vPoint, CKNNSimpleResultSet<float>& voResult);
		void   __searchKNearestNeighbor(const float *vQueries, unsigned int *voKIdices, float *voKDistance);
		void   __split(const AABBBoundingBox& vBBox, int *vIndices, int vNumInputPoint, int& voDivOffset, int& voDivAxis, float& voDivValue);
		void   __splitPlane(int vNumInputPoint, int vDivAxis, float vDivValue, int *vioIndices, int& voIndexLTDivValue, int& voIndexEorLTDivValue);
		void   __searchLevel(const float *vPoint, const SNode *vNode, float voMindistsq, std::vector<float>& vDists, CKNNSimpleResultSet<float>& voResult);
		float __computeInitialDistances(const float *vPoint, std::vector<float>& vDists);
		SNode* __divideTree(int vLeft, int vRight, AABBBoundingBox& voBbox);
		void   __deleteTree();
		void __destructData();

	private:
		struct SComputeDistance
		{
			template <typename Iterator1, typename Iterator2>
			float operator()(Iterator1 a, Iterator2 b, unsigned int vSize) const
			{
				float Result = 0;
				float Diff;
				for(unsigned int i=0; i<vSize; ++i)
				{
					Diff = *a++ - *b++;
					Result += Diff*Diff;
				}
				return Result;
			}

			template <typename U, typename V>
			float computeDistance(const U& vA, const V& vB) const
			{
				return (vA-vB)*(vA-vB);
			}
		};

	private:
		bool   m_IsKDTreeBuilt;
		SNode* m_pRootNode;
		float* m_pReorderData;
		unsigned int     m_NumPoint;
		unsigned int     m_Dimension;
		unsigned int     m_NumNeighbor;		
		AABBBoundingBox  m_RootBbox;
		SComputeDistance m_Distance;
		std::vector<int> m_IndexOfData;

		const int        m_LeafMaxSize;
		const float*     m_pInputData;


		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & m_IsKDTreeBuilt;
			ar & m_pRootNode;
			ar & m_pReorderData;
			ar & m_NumPoint;
			ar & m_Dimension;
			ar & m_NumNeighbor;
			ar & m_RootBbox;
			ar & m_IndexOfData;
			ar & m_LeafMaxSize;
			ar & m_pInputData;
		}
	};
}