#pragma once
#include <vector>
#include <glm/glm.hpp>
//#include <boost/serialization/serialization.hpp>
#include "common/WeightedDirectedGraph.h"
#include "BaseScene.h"

namespace hiveSceneProcess
{
	typedef hiveCommon::CWeightedDirectedGraph<glm::vec2>::GraphVertex GRAPHVERTEX;
	
	class CBaseScene;

	class SCENE_DLL_EXPORT CBaseGraph
	{
	public:
		CBaseGraph();
		virtual ~CBaseGraph();

		void  genGraph(const CBaseScene* vScene);
		float getEdgeWeight(const glm::vec2& vBeginNode, const glm::vec2& vEndNode) const;
		void  getAdjacentNodeSet(const glm::vec2& vTargetNode, std::vector<glm::vec2>& voAdjacentNodeSet) const;

	protected:
		virtual float _computeEdgeWeightV(const glm::vec2& vBegin, const glm::vec2& vEnd) const = 0;

	private:
		void        __addNode(glm::vec2 vGrid);
		void        __addEdge(const glm::vec2& vBeginNode, const glm::vec2& vEndNode, float vWeight);

		GRAPHVERTEX __node2Index(const glm::vec2& vTargetNode) const ;
		glm::vec2   __index2Node(const GRAPHVERTEX& vTargetIndex) const;
		void        __findAndAddEdgesForAGrid(glm::vec2& vNode);

	protected:
		hiveCommon::CWeightedDirectedGraph<glm::vec2>  m_Graph;
	    std::vector<std::pair<GRAPHVERTEX, glm::vec2>> m_Node2IndexVec; //其他容器，高效查找？？
	    const CBaseScene* m_pScene;

		//friend class boost::serialization::access;

		//template<class Archive>
		//void serialize(Archive& ar, const unsigned int version)
		//{
		//	
		//}

	};
}