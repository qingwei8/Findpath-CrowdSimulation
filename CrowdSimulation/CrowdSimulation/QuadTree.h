#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace hiveCrowdSimulation
{
// 	struct SRectRange
// 	{
// 		glm::vec2 m_PosA;
// 		glm::vec2 m_PosB;
// 		glm::vec2 m_PosC;
// 		glm::vec2 m_PosD;
// 	};

	enum EPassibility
	{
		PASS   = 0,
		NOPASS = 1,
		ORPASS = 2
	};

	struct SQuadTreeNode 
	{
		glm::vec2         m_MinPos;
		glm::vec2         m_MaxPos;
		EPassibility      m_Passibility;
		std::vector<char> m_CodeSet;
		std::vector<SQuadTreeNode*> m_ChildSet;
	};

	class CQuadTree
	{
	public:
		CQuadTree();
		~CQuadTree();

		void setInfo(){}
		void createTree();

	private:
		SQuadTreeNode* m_pRoot;
	};
}