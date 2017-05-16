#pragma once
#include <string>
#include "RenderingExport.h"

class CROWD_RENDERING_DLL_EXPORT CFileConfig
{
public:
	CFileConfig();
	~CFileConfig();

	virtual void parserV(const std::string& vConfigFile);
	float getSceneWidth() const {return m_SceneWidth;}
	float getSceneHeight() const {return m_SceneHeight;}
	std::string getModelName() const {return m_DataPath + m_ModelName;}
	std::string getVertShaderName() const {return m_DataPath + m_VertShaderName;}
	std::string getFragShaderName() const {return m_DataPath + m_FragShaderName;}

private:
	float m_SceneWidth;
	float m_SceneHeight;
	std::string m_DataPath;
	std::string m_ModelName;
	std::string m_VertShaderName;
	std::string m_FragShaderName;
};