#pragma once
#include "Visualization.h"
#include "RenderingExport.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace hiveCrowdRendering
{
	class CROWD_RENDERING_DLL_EXPORT CVisualizationByOpenCV : public CVisualization
	{
	public:
		CVisualizationByOpenCV(float vWidth, float vHeight, const cv::Scalar& vBackColor);
		~CVisualizationByOpenCV();

		virtual void clearScene() override;
		virtual void display() override;
		virtual void drawPoint(const glm::vec2& vPosition, float vRadius, const glm::vec3& vColor, bool vIsFilled) override;
		virtual void drawLine(const glm::vec2& vStart, const glm::vec2& vEnd, const glm::vec3& vColor, float vThickness) override;
		virtual void drawRectangle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom, const glm::vec3& vColor, bool vIsFilled) override;
		virtual void drawSphere(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor) override;
		virtual void drawCylinder(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor) override;
		virtual void drawCone(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor) override;

	private:
		cv::Mat m_Scene;
		float m_Width;
		float m_Height;
	};
}