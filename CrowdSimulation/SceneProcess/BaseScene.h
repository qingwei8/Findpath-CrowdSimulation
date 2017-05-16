#pragma once
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include "SceneCommon.h"
#include "SceneExport.h"

namespace hiveSceneProcess
{
	class CSceneGrid;

	class SCENE_DLL_EXPORT CBaseScene
	{
	public:
		CBaseScene();
		virtual ~CBaseScene();

		void setParameter(int vParameterA, int vParameterB);
		void generateScene(const std::string& vSceneImage);

		void dumpObstacleSet(std::vector<const SRectShape*>& voObstacleSet) const;
		void dumpPassableSet(std::vector<const SRectShape*>& voPassableSet) const;

		unsigned int getSceneWidthInPixel()  const {return m_SceneWidthInPixel;}
		unsigned int getSceneHeightInPixel() const {return m_SceneHeightInPixel;}
		unsigned int getNumExit()            const {return m_ExitSet.size();}
		glm::vec2 getExit(unsigned int vIndex) const;

		bool isPosInObstacle(const glm::vec2& vPos) const;
		bool isPositionInScene(const glm::vec2& vScenePos) const;

		virtual bool dumpPassableSetInsightV(const glm::vec2& vGridCenter, float vDistance, std::vector<const SRectShape*>& voGirdSet) const = 0;
		virtual void dumpAdjacentPassableSetV(const glm::vec2& vGridCenter, std::vector<const SRectShape*>& voAdjSet) const {}
		virtual bool isDirectVisibleV(const glm::vec2& vScenePosA, const glm::vec2& vScenePosB) const = 0;
		virtual CSceneGrid* getSceneGridV(const glm::vec2& vPosInScene) const = 0;
		virtual glm::vec2   getSceneGridCenterV(const glm::vec2& vPosInScene) const = 0;//Todo:有无必要？？

	protected:
		void _addPassableGrid(CSceneGrid* vSceneGrid);
		void _addObstacleGrid(CSceneGrid* vSceneGrid);

		virtual void _generateSceneV(const char* vImageData) {}

	private:
		void __addExit(const glm::vec2& vEixt);

	protected:
		int m_ParameterA;
		int m_ParameterB;
		int m_SceneWidthInPixel;
		int m_SceneHeightInPixel;
		bool m_IsDivideSceneInfoSpecified;

		std::vector<const SRectShape*> m_PassableGridSet;
		std::vector<const SRectShape*> m_ObstacleGirdSet;

		unsigned int m_PassableGridSize;
		unsigned int m_ObstacleGridSize;

	private:
		std::vector<glm::vec2> m_ExitSet;
		unsigned int m_NumExit;

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & m_ParameterA;
			ar & m_ParameterB;
			ar & m_SceneWidthInPixel;
			ar & m_SceneHeightInPixel;
			ar & m_IsDivideSceneInfoSpecified;
			ar & m_PassableGridSize;
			ar & m_ObstacleGridSize;
			ar & m_NumExit;

			if (m_PassableGridSet.empty()) m_PassableGridSet.resize(m_PassableGridSize);
			for (unsigned int i=0; i<m_PassableGridSet.size(); ++i)
			{
				ar & m_PassableGridSet[i];
			}

			if (m_ObstacleGirdSet.empty()) m_ObstacleGirdSet.resize(m_ObstacleGridSize);
			for (unsigned int i=0; i<m_ObstacleGirdSet.size(); ++i)
			{
				ar & m_ObstacleGirdSet[i];
			}

			if (m_ExitSet.empty()) m_ExitSet.resize(m_NumExit);
			for (unsigned int i=0; i<m_ExitSet.size(); ++i)
			{
				ar & m_ExitSet[i].x;
				ar & m_ExitSet[i].y;
			}
		}
	};
}