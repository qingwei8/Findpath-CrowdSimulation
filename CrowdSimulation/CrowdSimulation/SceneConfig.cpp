#include "SceneConfig.h"
#include "SimulationCommon.h"
#include "Common/ConfigParser.h"
#include "Common/CommonInterface.h"
#include "Common/HiveCommonMicro.h"

using namespace hiveCrowdSimulation;

CSceneConfig::CSceneConfig()
{
	__defineAttribute();
}

CSceneConfig::~CSceneConfig()
{
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CSceneConfig::parseV(const std::string& vAgentInfo)
{
	return hiveConfig::hiveParseConfig(vAgentInfo, hiveConfig::CONFIG_XML, this);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CSceneConfig::__defineAttribute()
{
	defineAttribute(SCENE_CONFIG_KEYWORD::SCENE_IMAGE,         hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(SCENE_CONFIG_KEYWORD::SCENE_GENERATE,      hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(SCENE_CONFIG_KEYWORD::UNIFORM_GRID_HEIGHT, hiveConfig::ATTRIBUTE_INT);
	defineAttribute(SCENE_CONFIG_KEYWORD::UNIFORM_GRID_WIDTH,  hiveConfig::ATTRIBUTE_INT);
	defineAttribute(SCENE_CONFIG_KEYWORD::BASENODE_DEEP,       hiveConfig::ATTRIBUTE_INT);
	defineAttribute(SCENE_CONFIG_KEYWORD::MINNODE_DEEP,        hiveConfig::ATTRIBUTE_INT);
}

//********************************************************************
//FUNCTION:
bool hiveCrowdSimulation::CSceneConfig::_verifyConfigV() const 
{
	if (!isAttributeExisted(SCENE_CONFIG_KEYWORD::SCENE_IMAGE))
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The configuration must specify the scene bmp image. The configuration of 'SceneImage' is missed.");
	    return false;
	}

	if (!isAttributeExisted(SCENE_CONFIG_KEYWORD::SCENE_GENERATE))
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The configuration must specify the scene divided method. The configuration of 'SceneGenerate' is missed.");
		return false;
	}

	std::string SceneGenerateMethod = getAttribute<std::string>(SCENE_CONFIG_KEYWORD::SCENE_GENERATE);
	if (strupr(const_cast<char*>(SceneGenerateMethod.c_str())) == SCENE_DIVIDE::UNIFORMGRID_DIVIDE)
	{
		if (!isAttributeExisted(SCENE_CONFIG_KEYWORD::UNIFORM_GRID_HEIGHT) || !isAttributeExisted(SCENE_CONFIG_KEYWORD::UNIFORM_GRID_WIDTH))
		{
			hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The divide information of uniform grid is missed. The configuration of 'DivideWidth' or 'DivideHeight' is missed.");
			return false;
		}
	}

	if (strupr(const_cast<char*>(SceneGenerateMethod.c_str())) == SCENE_DIVIDE::QUADTREE_DIVIDE)
	{
		if (!isAttributeExisted(SCENE_CONFIG_KEYWORD::BASENODE_DEEP) || !isAttributeExisted(SCENE_CONFIG_KEYWORD::MINNODE_DEEP))
		{
			hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "The divide information of quad tree is missed. The configuration of 'BaseNodeDeep' or 'MinNodeDeep' is missed.");
			return false;
		}
	}

	return true;
}