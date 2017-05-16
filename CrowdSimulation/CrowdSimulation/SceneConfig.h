#pragma once
#include "common/HiveConfig.h"
#include "common/Singleton.h"

namespace hiveCrowdSimulation
{
	class CSceneConfig : public hiveConfig::CHiveConfig, public hiveCommon::CSingleton<CSceneConfig>
	{
	public:
		~CSceneConfig();

		 bool parseV(const std::string& vAgentInfo);

	private:
		CSceneConfig();
		void __defineAttribute();
		virtual bool _verifyConfigV() const override;

		friend class hiveCommon::CSingleton<CSceneConfig>;
	};
}