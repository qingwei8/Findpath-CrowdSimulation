#include "CrowdSimulationConfig.h"
#include "common/ConfigParser.h"
#include "SimulationCommon.h"
#include "common/CommonInterface.h"
#include "common/HiveCommonMicro.h"

using namespace hiveCrowdSimulation;

CCrowdSimulationConfig::CCrowdSimulationConfig()
{
	__defineAttribute();
}

CCrowdSimulationConfig::~CCrowdSimulationConfig()
{
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CCrowdSimulationConfig::parseV(const std::string& vCrowdSimulationInfo)
{
	return hiveConfig::hiveParseConfig(vCrowdSimulationInfo, hiveConfig::CONFIG_XML, this);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulationConfig::__defineAttribute()
{
	defineAttribute(SIMULATION_CONFIG_KEYWORD::NUM_TOTAL_AGENT,      hiveConfig::ATTRIBUTE_INT);
	defineAttribute(SIMULATION_CONFIG_KEYWORD::IS_SAVE_SIMULATION_INFO, hiveConfig::ATTRIBUTE_BOOL);
	defineAttribute(SIMULATION_CONFIG_KEYWORD::SAVE_SIMULATION_FILE, hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(SIMULATION_CONFIG_KEYWORD::TIME_STEP,            hiveConfig::ATTRIBUTE_FLOAT);
	defineAttribute(SIMULATION_CONFIG_KEYWORD::AWARENESS_PERCENTAGE, hiveConfig::ATTRIBUTE_FLOAT);
	defineAttribute(SIMULATION_CONFIG_KEYWORD::IGNORANCE_PERCENTAGE, hiveConfig::ATTRIBUTE_FLOAT);
	defineAttribute(SIMULATION_CONFIG_KEYWORD::NUM_NO_GROUP_MEMBERS, hiveConfig::ATTRIBUTE_INT);

	defineAttribute(SIMULATION_CONFIG_KEYWORD::NUM_GROUPS,         hiveConfig::ATTRIBUTE_INT);
	defineAttribute(SIMULATION_CONFIG_KEYWORD::FLOOR_GROUPMEMBERS, hiveConfig::ATTRIBUTE_INT);
	defineAttribute(SIMULATION_CONFIG_KEYWORD::UPPER_GROUPMEMEBRS, hiveConfig::ATTRIBUTE_INT);

	defineAttribute(SIMULATION_CONFIG_KEYWORD::RADIUS_OF_GOAL, hiveConfig::ATTRIBUTE_FLOAT);

	defineAttribute(SIMULATION_CONFIG_KEYWORD::AWARENESS_AGENT_GIVEUP_TIME, hiveConfig::ATTRIBUTE_INT);
	defineAttribute(SIMULATION_CONFIG_KEYWORD::IGNORANT_AGENT_GIVEUP_TIME,  hiveConfig::ATTRIBUTE_INT);
	defineAttribute(SIMULATION_CONFIG_KEYWORD::GROUP_GIVEUP_TIME,          hiveConfig::ATTRIBUTE_INT);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CCrowdSimulationConfig::__onloadV()
{
	if (!isAttributeExisted(SIMULATION_CONFIG_KEYWORD::IS_SAVE_SIMULATION_INFO))
	{
		setAttribute(SIMULATION_CONFIG_KEYWORD::IS_SAVE_SIMULATION_INFO, false);
	}
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CCrowdSimulationConfig::_verifyConfigV() const 
{
	//Todo:其他验证
	if (getAttribute<bool>(SIMULATION_CONFIG_KEYWORD::IS_SAVE_SIMULATION_INFO) == true)
	{
		if (!isAttributeExisted(SIMULATION_CONFIG_KEYWORD::SAVE_SIMULATION_FILE))
		{
			hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The configuration must specify the file saving simulation result. The configuration of 'SaveSimulationFile' is missed.");
		    return false;
		}
	}

	return true;
}