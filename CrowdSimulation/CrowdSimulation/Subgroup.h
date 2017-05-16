#pragma once
#include "IAgent.h"

namespace hiveSceneProcess
{
	class CSubgroup
	{
	public:
		CSubgroup();
		~CSubgroup();

	private:
		std::vector<CBaseAgent*> m_SubgroupMembers;
	};
}