#pragma once
#include <Eigen/dense>
#include "IAgent.h"
#include "BaseScene.h"
#include "GroupCommon.h"

namespace hiveCrowdSimulation
{
	const float g_ConstantA   = 0.5;
	const float g_ConstantB   = 0.3;
	const float g_ConstantC   = 0.2;
	const float g_AwarenessG  = 2.0;
	const float g_IgnoranceG  = 1.0;
	const int g_TempGoalAdvanceFrameTime = 100;

	class CDerivedAgent : public hiveSceneProcess::CBaseAgent
	{
	public:
		CDerivedAgent(const glm::vec2& vPosition, const glm::vec2& vDirection);
		~CDerivedAgent();

		void setNeighborInfo(const std::vector<glm::vec2>& vNeighborPosSet, const std::vector<glm::vec2>& vNeighborVelSet, const std::vector<bool>& vNeighborAwareSet, std::vector<float>& vSpeedSet);
		void initAgentExtraInfo(unsigned int vRVOIndex, RVO::Agent* vRVOAgent, float vMoveSpeed, const glm::vec2& vGoalPos, hiveSceneProcess::CBaseScene* vScene);

		bool isExitSet2OldGoal() const;
		bool checkAroundExit(glm::vec2& voExitPos) const;
		glm::vec2 decideGoalPos();
		void decideGoalPosition();
		virtual bool updateFinishStateV(float vRadiusOfGoal) override;

		void resetAdvanceFrameTimes()     {m_AdvanceFrameTimes = 1;}
		void addAdvanceFrameTimes()       {m_AdvanceFrameTimes++;}
		int  getAdvanceFrameTiems() const {return m_AdvanceFrameTimes;}

		void updateNessarity2FindNewGoal(unsigned int vElapsedFrame);
		bool isNecessary2FindNewGoal() const {return m_IsNessary2FindNewGoal;}

		void setNeedComputeRoadMap(bool vIsNewGoalChoosed){m_IsNessary2ComputeRoadMap = vIsNewGoalChoosed;}
		bool isNeed2ComputeRoadMap() const {return m_IsNessary2ComputeRoadMap;}

		void addConcernedAgentInfo(SConcernedAgentInfo* vInfo);

		glm::vec2 computePreferVelocityCausedByTarget(float vReachTempGoalDis);

	protected:
		void _setScene(const hiveSceneProcess::CBaseScene* vScene);

	private:
		glm::vec2 __decideGoalDirByIgnorance();
		glm::vec2 __computeMatrixCol(unsigned int vNeighborIndex);
		glm::vec2 __computeNeighborInfluence();
		glm::vec2 __computeExitSignInfluence();
		glm::vec2 __chooseOneDirVisibleEixt();
		glm::vec2 __chooseOneRandomExit();

		bool __isAnyExitVisible() const;

		Eigen::MatrixXd __constructInfluenceMatrix();

		void __repairInfluenceNeighborSet();
		void __dumpNearestNeighbor(unsigned int vNumNearestNeighbor, std::vector<glm::vec2>& voNearNeighborPos, std::vector<glm::vec2>& voNearNeighborVel, std::vector<bool>& voNearNeighborAwareness, std::vector<float>& voNearNeighborSpeed);
		
		float __computeNeighborInfluenceWeight(unsigned int vNeighborIndex);
		float __computeDistanceInfluence(unsigned int vNeighborIndex);
		float __computeVelocityInfluence(const glm::vec2& vAvgVelocity, unsigned int vNeighborIndex);
		float __computePositionInfluence(const glm::vec2& vAvgVelocity, unsigned int vNeighborIndex);
		float __computeVelocityMagnitude();

		glm::vec2 __computeInfluenceSpace4Ignorance();
		float __decideCombinateWeight();
		int   __getNumAwareNeighbor() const;

	private:
		const hiveSceneProcess::CBaseScene* m_pScene;

		std::vector<glm::vec2> m_NeighborPosSet;
		std::vector<glm::vec2> m_NeighborVelSet;
		std::vector<bool>      m_NeighborIsAwareSet;
		std::vector<float>     m_NeighborMoveSpeedSet;
		unsigned int           m_NeighborSize;

		int m_AdvanceFrameTimes;

		bool m_IsNessary2FindNewGoal;
		bool m_IsNessary2ComputeRoadMap;

		std::vector<SConcernedAgentInfo*> m_ConcernedAgentSet;

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<CBaseAgent>(*this);
			ar & m_pScene;
			ar & m_IsNessary2FindNewGoal;
			ar & m_IsNewGoalChoosed;
			ar & m_NeighborSize;

			if (m_NeighborPosSet.empty()) m_NeighborPosSet.resize(m_NeighborSize);
			for (unsigned int i=0; i<m_NeighborPosSet.size(); ++i)
			{
				ar & m_NeighborPosSet[i].x;
				ar & m_NeighborPosSet[i].y;
			}

			if (m_NeighborVelSet.empty()) m_NeighborVelSet.resize(m_NeighborSize);
			for (unsigned int i=0; i<m_NeighborVelSet.size(); ++i)
			{
				ar & m_NeighborVelSet[i].x;
				ar & m_NeighborVelSet[i].y;
			}

			if (m_NeighborIsAwareSet.empty()) m_NeighborIsAwareSet.resize(m_NeighborSize);
			for (unsigned int i=0; i<m_NeighborIsAwareSet.size(); ++i)
			{
				ar & m_NeighborIsAwareSet[i];
			}

			if (m_NeighborMoveSpeedSet.empty()) m_NeighborMoveSpeedSet.resize(m_NeighborSize);
			for (unsigned int i=0; i<m_NeighborMoveSpeedSet.size(); ++i)
			{
				ar & m_NeighborMoveSpeedSet[i];
			}
		}
	};
}