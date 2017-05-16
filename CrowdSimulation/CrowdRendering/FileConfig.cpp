#include "FileConfig.h"
#include <fstream>
#include <vector>

CFileConfig::CFileConfig()
{
}

CFileConfig::~CFileConfig()
{
}

//*******************************************************************
//FUNCTION:
void CFileConfig::parserV(const std::string& vConfigFile)
{
	std::ifstream FileIO;
	FileIO.open(vConfigFile);

	_ASSERT(FileIO.is_open());

	std::string LineStr;
	std::vector<std::string> SplitStr;

	while (std::getline(FileIO, LineStr))
	{
		if (LineStr == "SceneWidth")
		{
			std::getline(FileIO, LineStr);
			m_SceneWidth = atof(LineStr.c_str());
		}

		if (LineStr == "SceneHeight")
		{
			std::getline(FileIO, LineStr);
			m_SceneHeight = atof(LineStr.c_str());
		}

		if (LineStr == "DataPath")
		{
			std::getline(FileIO, LineStr);
			m_DataPath = LineStr;
		}
		if (LineStr == "ModelName")
		{
			std::getline(FileIO, LineStr);
			m_ModelName = LineStr.c_str();
		}

		if (LineStr == "VertShaderName")
		{
			std::getline(FileIO, LineStr);
			m_VertShaderName = LineStr;
		}

		if (LineStr == "FragShaderName")
		{
			std::getline(FileIO, LineStr);
			m_FragShaderName = LineStr;
		}
	}
}
