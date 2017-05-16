#include "ColorConfig.h"
#include "common/ConfigParser.h"
#include "SimulationCommon.h"

using namespace hiveCrowdSimulation;

CColorConfig::CColorConfig()
{
	__defineAttribute();
}

CColorConfig::~CColorConfig()
{
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CColorConfig::parseV(const std::string& vColorInfo)
{
	_ASSERT(!vColorInfo.empty());
	return hiveConfig::hiveParseConfig(vColorInfo, hiveConfig::CONFIG_XML, this);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CColorConfig::__defineAttribute()
{
	defineAttribute(COLOR_CONFIG_KEYWORD::BACKGROUND_COLOR,      hiveConfig::ATTRIBUTE_VEC3F);
	defineAttribute(COLOR_CONFIG_KEYWORD::EXIT_COLOR,            hiveConfig::ATTRIBUTE_VEC3F);
	defineAttribute(COLOR_CONFIG_KEYWORD::OBSTACLE_GRID_COLOR,   hiveConfig::ATTRIBUTE_VEC3F);
	defineAttribute(COLOR_CONFIG_KEYWORD::PASS_GRID_COLOR,       hiveConfig::ATTRIBUTE_VEC3F);
	defineAttribute(COLOR_CONFIG_KEYWORD::AGENT_AWARENESS_COLOR, hiveConfig::ATTRIBUTE_VEC3F);
	defineAttribute(COLOR_CONFIG_KEYWORD::AGENT_IGNORANT_COLOR,  hiveConfig::ATTRIBUTE_VEC3F);
	defineAttribute(COLOR_CONFIG_KEYWORD::AGENT_COLOR,           hiveConfig::ATTRIBUTE_VEC3F);
	defineAttribute(COLOR_CONFIG_KEYWORD::GROUP_COLOR,           hiveConfig::ATTRIBUTE_VEC3F);
}

//********************************************************************
//FUNCTION:
glm::vec3 hiveCrowdSimulation::CColorConfig::__getColor(const std::string& vAttr) const
{
	_ASSERT(!vAttr.empty());
	boost::tuple<float, float, float> Color = this->getAttribute<boost::tuple<double, double, double>>(vAttr);
	return glm::vec3(Color.get<0>(), Color.get<1>(), Color.get<2>());
}

//********************************************************************
//FUNCTION:
glm::vec3 hiveCrowdSimulation::CColorConfig::getBackGroundColor() const
{
	return __getColor(COLOR_CONFIG_KEYWORD::BACKGROUND_COLOR);
}

//********************************************************************
//FUNCTION:
glm::vec3 hiveCrowdSimulation::CColorConfig::getExitColor() const
{
	return __getColor(COLOR_CONFIG_KEYWORD::EXIT_COLOR);
}

//********************************************************************
//FUNCTION:
glm::vec3 hiveCrowdSimulation::CColorConfig::getAgentIgnorantColor() const
{
	return __getColor(COLOR_CONFIG_KEYWORD::AGENT_IGNORANT_COLOR);
}

//********************************************************************
//FUNCTION:
glm::vec3 hiveCrowdSimulation::CColorConfig::getAgentAwareColor() const
{
	return __getColor(COLOR_CONFIG_KEYWORD::AGENT_AWARENESS_COLOR);
}

//********************************************************************
//FUNCTION:
glm::vec3 hiveCrowdSimulation::CColorConfig::getObstacleGridColor() const
{
	return __getColor(COLOR_CONFIG_KEYWORD::OBSTACLE_GRID_COLOR);
}

//********************************************************************
//FUNCTION:
glm::vec3 hiveCrowdSimulation::CColorConfig::getAgentColor() const
{
	return __getColor(COLOR_CONFIG_KEYWORD::AGENT_COLOR);
}

//********************************************************************
//FUNCTION:
glm::vec3 hiveCrowdSimulation::CColorConfig::getPassGridColor() const
{
	return __getColor(COLOR_CONFIG_KEYWORD::PASS_GRID_COLOR);
}

//********************************************************************
//FUNCTION:
glm::vec3 hiveCrowdSimulation::CColorConfig::getGroupColor() const
{
	return __getColor(COLOR_CONFIG_KEYWORD::GROUP_COLOR);
}