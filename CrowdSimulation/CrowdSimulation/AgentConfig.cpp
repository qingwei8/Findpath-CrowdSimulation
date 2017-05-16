#include "AgentConfig.h"
#include "common/ConfigParser.h"
#include "SimulationCommon.h"

using namespace hiveCrowdSimulation;

CAgentConfig::CAgentConfig()
{
	__defineAttribute();
}

CAgentConfig::~CAgentConfig()
{
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CAgentConfig::parseV(const std::string& vAgentInfo)
{
	return hiveConfig::hiveParseConfig(vAgentInfo, hiveConfig::CONFIG_XML, this);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CAgentConfig::__defineAttribute()
{
	defineAttribute(AGENT_CONFIG_KEYWORD::RADIUS,    hiveConfig::ATTRIBUTE_FLOAT);
	defineAttribute(AGENT_CONFIG_KEYWORD::NEIGHBOR_DIST, hiveConfig::ATTRIBUTE_FLOAT);
	defineAttribute(AGENT_CONFIG_KEYWORD::MAX_SPEED, hiveConfig::ATTRIBUTE_FLOAT);
	
	defineAttribute(AGENT_CONFIG_KEYWORD::TIME_HORIZON,     hiveConfig::ATTRIBUTE_FLOAT);
	defineAttribute(AGENT_CONFIG_KEYWORD::TIME_HORIZON_OBST, hiveConfig::ATTRIBUTE_FLOAT);

	defineAttribute(AGENT_CONFIG_KEYWORD::VISIBLE_RANGE,   hiveConfig::ATTRIBUTE_FLOAT);
}