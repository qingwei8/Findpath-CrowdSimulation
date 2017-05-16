#pragma once

namespace hiveCrowdSimulation
{
	struct SAgentRelationship
	{
		float Intimacy;
		float Visibility;
		float Attention;

		SAgentRelationship() : Intimacy(0.0), Visibility(0.0), Attention(0.0) {}
	};

	class CDerivedAgent;
	struct SConcernedAgentInfo
	{
		CDerivedAgent* pConcernedAgent;
		glm::vec2      LastCheckedPosition;
		int            LastCheckedTimeStep;
	};
}