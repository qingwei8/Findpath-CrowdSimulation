#include "OpenGLConfig.h"
#include "common/ConfigParser.h"


hiveCrowdRendering::COpenGLConfig::COpenGLConfig(void)
{
	defineAttribute(SEPHEREMODEL,  hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(CYLINDERMODEL, hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(SCENEVERTEX,   hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(SCENEFRAG,	   hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(OBJECTVERTEX,  hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(OBJECTFRAG,	   hiveConfig::ATTRIBUTE_STRING);
}

hiveCrowdRendering::COpenGLConfig::~COpenGLConfig(void)
{
}

//*******************************************************************
//FUNCTION:
bool hiveCrowdRendering::COpenGLConfig::parserV(const std::string& vOpenGLInfo)
{
	return hiveConfig::hiveParseConfig(vOpenGLInfo, hiveConfig::CONFIG_XML, this);
}
