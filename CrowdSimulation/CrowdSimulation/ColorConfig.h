#pragma once
#include <glm/glm.hpp>
#include "common/Singleton.h"
#include "common/HiveConfig.h"
#include "SimulationExport.h"

namespace hiveCrowdSimulation
{
	class CROWD_SIMULATION_DLL_EXPORT CColorConfig : public hiveConfig::CHiveConfig, public hiveCommon::CSingleton<CColorConfig>
	{
	public:
		~CColorConfig();

		 bool parseV(const std::string& vColorInfo);

		glm::vec3 getBackGroundColor()    const;
		glm::vec3 getExitColor()          const;
		glm::vec3 getObstacleGridColor()  const;
		glm::vec3 getPassGridColor()      const;
		glm::vec3 getAgentIgnorantColor() const;
		glm::vec3 getAgentAwareColor()    const;
		glm::vec3 getAgentColor()         const;
		glm::vec3 getGroupColor()         const;

	private:
		CColorConfig();

		void __defineAttribute();
		glm::vec3 __getColor(const std::string& vAttr) const;

		friend class hiveCommon::CSingleton<CColorConfig>;
	};
}