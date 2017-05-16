#pragma once
#include <GLM/glm.hpp>

namespace hiveCrowdRendering
{
	class CVisualization
	{
	public:
		CVisualization(void);
		virtual ~CVisualization(void);

		virtual void clearScene() = 0;
		virtual void display() = 0;
		virtual void drawPoint(const glm::vec2& vPosition, float vRadius, const glm::vec3& vColor, bool vIsFilled) = 0;
		virtual void drawLine(const glm::vec2& vStart, const glm::vec2& vEnd, const glm::vec3& vColor, float vThickness) = 0;
		virtual void drawRectangle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom, const glm::vec3& vColor, bool vIsFilled) = 0;
		virtual void drawSphere(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor) = 0;
		virtual void drawCylinder(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor) = 0;
		virtual void drawCone(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor) = 0;
	};
}