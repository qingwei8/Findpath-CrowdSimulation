#include "VisualizationByOpenGL.h"
#include "OpenGLConfig.h"
#include "Model.h"
#include "Shader.h"
#include <gl/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Magick++.h>

hiveCrowdRendering::CVisualizationByOpenGL::CVisualizationByOpenGL(float vWidth, float vHeight, const std::string& vSceneModelPath)
{
	__initWindow(vWidth, vHeight);
	__initObserveData();

	__loadSceneModel(vSceneModelPath);
	_ASSERT(COpenGLConfig::getInstance()->parserV("../../CrowdRendering/OpenglConfig.xml"));
	__loadSphereModel(COpenGLConfig::getInstance()->getSephereModel());
	__loadCylinderModel(COpenGLConfig::getInstance()->getCylinderModel());

	__loadSceneShader(COpenGLConfig::getInstance()->getSceneVertex(), COpenGLConfig::getInstance()->getSceneFrag());
	__loadObjectShader(COpenGLConfig::getInstance()->getObjectVertex(), COpenGLConfig::getInstance()->getObjectFrag());
}

hiveCrowdRendering::CVisualizationByOpenGL::~CVisualizationByOpenGL(void)
{
	glfwTerminate();
	__destructData();
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::clearScene()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::display()
{
	drawSphere(glm::vec2(256, 0), 3.0, glm::vec3(1.0, 0.0, 0.0));
	__drawScene();
	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::drawSphere(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor)
{
	__drawObject(m_pSphereModel, m_pObjectShader, vPos, vRadius, vColor);
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::drawCylinder(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor)
{
	__drawObject(m_pCylinderModel, m_pObjectShader, vPos, vRadius, vColor);
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::drawCone(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor)
{
	//
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::drawPoint(const glm::vec2& vPosition, float vRadius, const glm::vec3& vColor, bool vIsFilled)
{
	//
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::drawLine(const glm::vec2& vStart, const glm::vec2& vEnd, const glm::vec3& vColor, float vThickness)
{
	//
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::drawRectangle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom, const glm::vec3& vColor, bool vIsFilled)
{
	//
}

//************************************************************
//FUNCTION:
bool hiveCrowdRendering::CVisualizationByOpenGL::__initWindow(float vWidth, float vHeight)
{
	if (!glfwInit())
		return false;

	m_pWindow = glfwCreateWindow(vWidth, vHeight, "CrowdSimulation", NULL, NULL);

	if (!m_pWindow)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_pWindow);

	GLuint res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false;
	}

	return true;
}


//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::__initObserveData()
{
	m_ModelMatrix = glm::mat4(1.0);
	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(0.01, 0.01, 0.01));
	m_ProjectionMatrix = glm::perspective(45.0f, 1.0f / 1.0f, 0.1f, 100.0f);
	m_ViewMatrix	   = glm::lookAt(glm::vec3(0, 6, 4), glm::vec3(0.0, 0.0, 0), glm::vec3(0.0, 1.0, 0.0));
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::__destructData()
{
	delete m_pSceneModel;
	delete m_pSphereModel;
	delete m_pCylinderModel;

	delete m_pSceneShader;
	delete m_pObjectShader;
}

//************************************************************
//FUNCTION:
bool hiveCrowdRendering::CVisualizationByOpenGL::__loadSceneModel(const std::string& vFilePath)
{
	m_pSceneModel = new CModel(vFilePath);
	return m_pSceneModel == NULL ? false : true;
}

//************************************************************
//FUNCTION:
bool hiveCrowdRendering::CVisualizationByOpenGL::__loadSphereModel(const std::string& vFilePath)
{
	m_pSphereModel = new CModel(vFilePath);
	return m_pSphereModel == NULL? false : true;
}

//************************************************************
//FUNCTION:
bool hiveCrowdRendering::CVisualizationByOpenGL::__loadCylinderModel(const std::string& vFilePath)
{
	m_pCylinderModel = new CModel(vFilePath);
	return m_pCylinderModel == NULL ? false : true;
}

//************************************************************
//FUNCTION:
bool hiveCrowdRendering::CVisualizationByOpenGL::__loadSceneShader(const std::string& vVertexPath, const std::string& vFragPath)
{
	m_pSceneShader = new CShader(vVertexPath, vFragPath);
	return m_pSceneShader == NULL ? false : true;
}

//************************************************************
//FUNCTION:
bool hiveCrowdRendering::CVisualizationByOpenGL::__loadObjectShader(const std::string& vVertexPath, const std::string& vFragPath)
{
	m_pObjectShader = new CShader(vVertexPath, vFragPath);
	return m_pObjectShader == NULL ? false : true;
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::__drawScene()
{
	m_pSceneShader->use();
	
	GLint ModelMatrixLoc	  = m_pSceneShader->getUniformLocation("uModelMatrix");
	GLint ViewMatrixLoc		  = m_pSceneShader->getUniformLocation("uViewMatrix");
	GLint ProjectionMatrixLoc = m_pSceneShader->getUniformLocation("uProjectionMatrix");

	m_pSceneShader->setUniformMatrix4fv(ModelMatrixLoc, glm::value_ptr(m_ModelMatrix));
	m_pSceneShader->setUniformMatrix4fv(ViewMatrixLoc, glm::value_ptr(m_ViewMatrix));
	m_pSceneShader->setUniformMatrix4fv(ProjectionMatrixLoc, glm::value_ptr(m_ProjectionMatrix));

	m_pSceneModel->draw(m_pSceneShader);
	m_pSceneShader->disable();
}

//************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenGL::__drawObject(CModel* vModel, CShader* vShader, const glm::vec2& vPos, float vRadius, const glm::vec3& vColor)
{
	vShader->use();
	glm::mat4 ModelMatrix = m_ModelMatrix;
	ModelMatrix			  = glm::translate(ModelMatrix, glm::vec3(vPos.x, 6.0, vPos.y));
	ModelMatrix		      = glm::scale(ModelMatrix, glm::vec3(0.5, 0.5, 0.5) * vRadius);

	GLint ModelMatrixLoc	  = vShader->getUniformLocation("uModelMatrix");
	GLint ViewMatrixLoc		  = vShader->getUniformLocation("uViewMatrix");
	GLint ProjectionMatrixLoc = vShader->getUniformLocation("uProjectionMatrix");
	GLint ObjectColorLoc	  = vShader->getUniformLocation("uLightColor");

	vShader->setUniformMatrix4fv(ModelMatrixLoc, glm::value_ptr(ModelMatrix));
	vShader->setUniformMatrix4fv(ViewMatrixLoc, glm::value_ptr(m_ViewMatrix));
	vShader->setUniformMatrix4fv(ProjectionMatrixLoc, glm::value_ptr(m_ProjectionMatrix));
	vShader->setUniform3fv(ObjectColorLoc, glm::value_ptr(vColor));

	vModel->draw(vShader);
	vShader->disable();
}