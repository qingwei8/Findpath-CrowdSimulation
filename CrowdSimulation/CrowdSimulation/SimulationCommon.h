#pragma once
#include <string>
#include <glm/glm.hpp>
#include <Eigen/dense>
#include "common/CommonInterface.h"

namespace hiveCrowdSimulation
{
// 	enum EPassibility
// 	{
// 		PASS   = 0,
// 		NOPASS = 1,
// 		ORPASS = 2
// 	};

// 	enum EDisplay
// 	{
// 		OpenCV = 0,
// 		OpenGL,
// 	};

	const unsigned int g_MaxNeighbors   = 6;
	const unsigned int g_Dimension      = 2;
	const float        g_AgentReachGoalRange = 10.0;
	const float        g_GroupMemberReachGoalRange = 15.0;

	namespace AGENT_CONFIG_KEYWORD
	{
		const std::string RADIUS            = "Radius";
		const std::string MAX_SPEED         = "MaxSpeed";
		const std::string VISIBLE_RANGE     = "VisibleRange";
		const std::string NEIGHBOR_DIST     = "NeighborDist";
		const std::string TIME_HORIZON      = "TimeHorizon";
		const std::string TIME_HORIZON_OBST = "TimeHorizonObst";
	}

	namespace SCENE_CONFIG_KEYWORD
	{
		const std::string SCENE_IMAGE          = "SceneImage";
		const std::string SCENE_GENERATE       = "SceneGenerate";
		const std::string UNIFORM_GRID_WIDTH   = "UniformGridWidth";
		const std::string UNIFORM_GRID_HEIGHT  = "UniformGridHeight";
		const std::string BASENODE_DEEP        = "BaseNodeDeep";
		const std::string MINNODE_DEEP         = "MinNodeDeep";
	}

	namespace SIMULATION_CONFIG_KEYWORD
	{
		const std::string IS_SAVE_SIMULATION_INFO  = "IsSaveSimulationInfo";
		const std::string SAVE_SIMULATION_FILE     = "SaveSimulationFile";
		const std::string TIME_STEP                = "TimeStep";
		const std::string NUM_TOTAL_AGENT	       = "NumTotalAgent";
		const std::string AWARENESS_PERCENTAGE	   = "AwarenessPercentage";
		const std::string IGNORANCE_PERCENTAGE	   = "IgnorancePercentage";
		const std::string NUM_NO_GROUP_MEMBERS     = "NumNoGroupMembers";
		const std::string NUM_GROUPS               = "NumGroups";
		const std::string FLOOR_GROUPMEMBERS       = "FloorGroupMembers";
		const std::string UPPER_GROUPMEMEBRS       = "UperGroupMembers";
		const std::string RADIUS_OF_GOAL           = "RadiusOfGoal";

		const std::string AWARENESS_AGENT_GIVEUP_TIME = "AwarenessAgentGiveupTime";
		const std::string IGNORANT_AGENT_GIVEUP_TIME  = "IgnorantAgentGiveupTime";
		const std::string GROUP_GIVEUP_TIME           = "GroupGiveupTime";
	}

	namespace COLOR_CONFIG_KEYWORD
	{
		const std::string BACKGROUND_COLOR      = "BackGroundColor";
		const std::string EXIT_COLOR            = "ExitColor";
		const std::string OBSTACLE_GRID_COLOR   = "ObstacleGridColor";
		const std::string PASS_GRID_COLOR       = "PassGridColor";
		const std::string AGENT_COLOR           = "AgentColor";
		const std::string AGENT_AWARENESS_COLOR = "AgentAwarenessColor";
		const std::string AGENT_IGNORANT_COLOR  = "AgentIgnorantColor";
		const std::string GROUP_COLOR           = "GroupColor";
	}

	namespace SCENE_DIVIDE
	{
		const std::string UNIFORMGRID_DIVIDE = "UNIFORMGRID";
		const std::string QUADTREE_DIVIDE    = "QUADTREE";
	}

	static int exeRoulette(const std::vector<float>& vPossabilitySet)
	{
		std::vector<float> IndexSet;
		float Sum = 0.0;
		for (unsigned int i=0; i<vPossabilitySet.size(); ++i)
		{
			Sum += vPossabilitySet[i];
			IndexSet.push_back(Sum);
		}

		float RandomIndex = hiveCommon::hiveGenRandomFloat(0, Sum);

		for (unsigned int i=0; i<IndexSet.size(); ++i)
		{
			if (RandomIndex <= IndexSet[i]) return i;
		}
		return -1;
	}
}