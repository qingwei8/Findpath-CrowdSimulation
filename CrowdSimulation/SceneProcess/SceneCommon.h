#pragma once
#include <glm/glm.hpp>
#include <boost/serialization/serialization.hpp>

namespace hiveSceneProcess
{
	struct SRectShape
	{
		glm::vec2 m_LeftUp;
		glm::vec2 m_RightDown;

		SRectShape(const glm::vec2& vLeftUp, const glm::vec2& vRightDown) : m_LeftUp(vLeftUp), m_RightDown(vRightDown)
		{
		}

		glm::vec2 getCenter() const
		{
			glm::vec2 Sum = m_LeftUp + m_RightDown;
			return glm::vec2(Sum.x / 2.0, Sum.y / 2.0);
		}

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & m_LeftUp.x;
			ar & m_LeftUp.y;
			ar & m_RightDown.x;
			ar & m_RightDown.y
		}
	};

	static void compareTwoNumber(float vA, float vB, float& voMax, float& voMin)
	{
		voMin = vA < vB ? vA : vB;
		voMax = vA < vB ? vB : vA;
	}

	struct SCompareFunction
	{
		bool operator () (const glm::vec2& vFirst, const glm::vec2& vSecond) const
		{
			return vFirst.x != vSecond.x ? vFirst.x < vSecond.x : vFirst.y < vSecond.y;
		}
	};
}