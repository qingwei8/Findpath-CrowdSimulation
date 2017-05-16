#pragma once
#include "common/HiveConfig.h"
#include "common/Singleton.h"

namespace hiveCrowdSimulation
{
	class CCrowdSimulationConfig : public hiveConfig::CHiveConfig, public hiveCommon::CSingleton<CCrowdSimulationConfig>
	{
	public:
		~CCrowdSimulationConfig();

		 bool parseV(const std::string& vCrowdSimulationInfo);

	private:
		CCrowdSimulationConfig();
		void __defineAttribute();
		virtual void __onloadV();
		virtual bool _verifyConfigV() const override;

		friend class hiveCommon::CSingleton<CCrowdSimulationConfig>;
	};
}