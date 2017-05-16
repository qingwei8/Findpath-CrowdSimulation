#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Visualization.h"
#include "RenderingExport.h"

namespace hiveCrowdRendering
{
	class CLight;
	class CModel;
	class CShader;

	class CROWD_RENDERING_DLL_EXPORT CVisualizationByOpenGL : public CVisualization
	{
	public:
		CVisualizationByOpenGL(float vWidth, float vHeight, const std::string& vSceneModelPath);
		~CVisualizationByOpenGL(void);

		virtual void clearScene() override;
		virtual void display() override;
		virtual void drawPoint(const glm::vec2& vPosition, float vRadius, const glm::vec3& vColor, bool vIsFilled) override;
		virtual void drawLine(const glm::vec2& vStart, const glm::vec2& vEnd, const glm::vec3& vColor, float vThickness) override;
		virtual void drawRectangle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom, const glm::vec3& vColor, bool vIsFilled) override;
		virtual void drawSphere(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor) override;
		virtual void drawCylinder(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor) override;
		virtual void drawCone(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor) override;

	private:
		bool __initWindow(float vWidth, float vHeight);
		void __initObserveData();
		void __destructData();
		bool __loadSceneModel(const std::string& vFilePath);
		bool __loadSphereModel(const std::string& vFilePath);
		bool __loadCylinderModel(const std::string& vFilePath);
		//bool __loadConeModel(const std::string& vFilePath);
		bool __loadSceneShader(const std::string& vVertexPath, const std::string& vFragPath);
		bool __loadObjectShader(const std::string& vVertexPath, const std::string& vFragPath);
		void __drawScene();
		void __drawObject(CModel* vModel, CShader* vShader, const glm::vec2& vPos, float vRadius, const glm::vec3& vColor);

	private:
		glm::mat4 m_ModelMatrix, m_ProjectionMatrix, m_ViewMatrix;
		GLFWwindow* m_pWindow;
		CModel*		m_pSceneModel;
		CModel*		m_pSphereModel;
		CModel*		m_pCylinderModel;
		CShader*	m_pSceneShader;
		CShader*	m_pObjectShader;
	};
}