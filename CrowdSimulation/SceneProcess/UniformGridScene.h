#pragma once
#include "BaseScene.h"
#include "SceneGrid.h"

namespace hiveSceneProcess
{
	struct SStraightLine
	{
		glm::vec2 PointA;
		glm::vec2 PointB;

		SStraightLine() {}
		SStraightLine(const glm::vec2& vPointA, const glm::vec2& vPointB) : PointA(vPointA), PointB(vPointB) {}

		float computeCorrespondingY(float vX) const
		{
			if (PointA.x == PointB.x)
				return -1;

			return (vX - PointA.x) * (PointB.y - PointA.y) / (PointB.x - PointA.x) + PointA.y;
		}

		float computeCorrespondingX(float vY) const
		{
			if (PointA.y == PointB.y)
				return -1;
			
			return (vY - PointA.y) * (PointB.x - PointA.x) / (PointB.y - PointA.y) + PointA.x;
		}

		float getSlope() const {return (PointA.y - PointB.y) / (PointA.x - PointB.x);}
		float getMaxX() const {return PointA.x > PointB.x ? PointA.x : PointB.x;}
		float getMinX() const {return PointA.x < PointB.x ? PointA.x : PointB.x;}
		float getMaxY() const {return PointA.y > PointB.y ? PointA.y : PointB.y;}
		float getMinY() const {return PointA.y < PointB.y ? PointA.y : PointB.y;}
	};

	class SCENE_DLL_EXPORT CUniformGridScene : public CBaseScene
	{
	public:
		CUniformGridScene();
		~CUniformGridScene();

	public:
		virtual bool dumpPassableSetInsightV(const glm::vec2& vGridCenter, float vDistance, std::vector<const SRectShape*>& voGirdSet) const;
		virtual void dumpAdjacentPassableSetV(const glm::vec2& vGridCenter, std::vector<const SRectShape*>& voAdjSet) const override;
		virtual CSceneGrid* getSceneGridV(const glm::vec2& vPosInScene) const override;
		virtual glm::vec2   getSceneGridCenterV(const glm::vec2& vPosInScene) const override;
		virtual bool isDirectVisibleV(const glm::vec2& vScenePosA, const glm::vec2& vScenePosB) const override;
		void computeGridCoordBetweenTwoGrids(const glm::vec2& vScenePosA, const glm::vec2& vScenePosB, std::vector<glm::vec2>& voScenePosResult) const;
		void testFunction();

	protected:
		virtual void _generateSceneV(const char* vImageData) override;

	private:
		void __handleXArrayCoordEqual(const glm::vec2& vArrayCoordA, const glm::vec2& vArrayCoordB, std::vector<glm::vec2>& voArrayCoordResult) const;
		void __handleYArrayCoordEqual(const glm::vec2& vArrayCoordA, const glm::vec2& vArrayCoordB, std::vector<glm::vec2>& voArrayCoordResult) const;
		void __handleEqualSlope(const glm::vec2& vArrayCoordA, const glm::vec2& vArrayCoordB, std::vector<glm::vec2>& voArrayCoordResult) const;//45度，斜率为正
		void __handleReverseSlope(const glm::vec2& vArrayCoordA, const glm::vec2& vArrayCoordB, std::vector<glm::vec2>& voArrayCoordResult) const;//135度，斜率为负
		void __handleOtherCases(const glm::vec2& vScenePosA, const glm::vec2& vScenePosB, std::vector<glm::vec2>& voArrayCoordResult) const;
		bool __isSegmentAndRectangleIntersect(const SStraightLine& vLine, const SRectShape* vNode) const;
		glm::uvec2 __getSceneGridCoord(const glm::vec2& vScenePos) const;
		glm::vec2  __getArrayCoord(const glm::vec2& vScenePos) const;

	private:
		std::vector<std::vector<CSceneGrid*>> m_SceneGridSet;

		unsigned int m_SizeOfWidth;
		unsigned int m_SizeOfHeight;

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<CBaseScene>(*this);
			ar & m_SizeOfWidth;
			ar & m_SizeOfHeight;

			if (m_SceneGridSet.empty()) m_SceneGridSet.resize(m_SizeOfHeight);
			for (unsigned int i=0; i<m_SceneGridSet.size(); ++i)
			{
				if (m_SceneGridSet[i].empty()) m_SceneGridSet[i].resize(m_SizeOfWidth);
				for (unsigned int k=0; k<m_SceneGridSet[i].size(); ++k)
				{
					ar & m_SceneGridSet[i][k];
				}
			}
		}
	};
}