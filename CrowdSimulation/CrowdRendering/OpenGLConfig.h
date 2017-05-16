#pragma once
#include <string>
#include <GLM/glm.hpp>
#include "common/HiveConfig.h"
#include "common/Singleton.h"

namespace hiveCrowdRendering
{
	const std::string SEPHEREMODEL	= "SephereModel";
	const std::string CYLINDERMODEL	= "CylinderModel";
	const std::string SCENEVERTEX	= "SceneVertex";
	const std::string SCENEFRAG		= "SceneFrag";
	const std::string OBJECTVERTEX	= "ObjectVertex";
	const std::string OBJECTFRAG	= "ObjectFrag";

	class COpenGLConfig: public hiveConfig::CHiveConfig, public hiveCommon::CSingleton<COpenGLConfig>
	{
	public:
		virtual ~COpenGLConfig(void);

		virtual bool parserV(const std::string& vOpenGLInfo);
		std::string getSephereModel()  const {return getAttribute<std::string>(SEPHEREMODEL);}
		std::string getCylinderModel() const {return getAttribute<std::string>(CYLINDERMODEL);}
		std::string getSceneVertex()   const {return getAttribute<std::string>(SCENEVERTEX);}
		std::string getSceneFrag()	   const {return getAttribute<std::string>(SCENEFRAG);}
		std::string getObjectVertex()  const {return getAttribute<std::string>(OBJECTVERTEX);}
		std::string getObjectFrag()	   const {return getAttribute<std::string>(OBJECTFRAG);}

	protected:
		COpenGLConfig(void);

	private:
		friend class hiveCommon::CSingleton<COpenGLConfig>;
	};
}

