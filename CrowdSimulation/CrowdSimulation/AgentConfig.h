#pragma once
#include "common/HiveConfig.h"
#include "common/Singleton.h"

namespace hiveCrowdSimulation
{
	class CAgentConfig : public hiveConfig::CHiveConfig, public hiveCommon::CSingleton<CAgentConfig>
	{
	public:
		~CAgentConfig();

		 bool parseV(const std::string& vAgentInfo);

	private:
		CAgentConfig();
		void __defineAttribute();

		friend class hiveCommon::CSingleton<CAgentConfig>;
	};
}