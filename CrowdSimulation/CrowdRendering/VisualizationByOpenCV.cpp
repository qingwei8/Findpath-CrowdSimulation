#include "VisualizationByOpenCV.h"
#include "FreeImage.h"

#define REVISE_COLOR 100

hiveCrowdRendering::CVisualizationByOpenCV::CVisualizationByOpenCV(float vWidth, float vHeight, const cv::Scalar& vBackColor)
{
	_ASSERT(vWidth > 0 && vHeight > 0);

	m_Width  = vWidth;
	m_Height = vHeight;
	m_Scene  = cv::Mat(vWidth, vWidth, CV_8SC3, vBackColor);
}

hiveCrowdRendering::CVisualizationByOpenCV::~CVisualizationByOpenCV()
{
}

//*******************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenCV::drawPoint(const glm::vec2& vPosition, float vRadius, const glm::vec3& vColor, bool vIsFilled)
{
	cv::Point Center = cv::Point(vPosition.x, vPosition.y);
	if (vIsFilled)
	{
		cv::circle(m_Scene, Center, vRadius, cv::Scalar(vColor.x, vColor.y, vColor.z), CV_FILLED); 
	}
	else
	{
		cv::circle(m_Scene, Center, vRadius, cv::Scalar(vColor.x, vColor.y, vColor.z));
	}
}

//*******************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenCV::display() 
{
	static int ScreenShotCount = 0;
	cv::imshow("Scene", m_Scene);
	cvMoveWindow("Scene", 10, 10); //set window position
	cv::waitKey(1);

	//BYTE* pPixels = new BYTE[3*m_Scene.rows*m_Scene.cols];
	//int item =0;

	//for (int i=0; i<m_Scene.rows; ++i)
	//{
	//	for (int k=0; k<m_Scene.cols; ++k)
	//	{
	//		pPixels[item] = m_Scene.at<cv::Vec3b>(i, k)[0] + REVISE_COLOR;
	//		pPixels[item+1] = m_Scene.at<cv::Vec3b>(i, k)[1] + REVISE_COLOR;
	//		pPixels[item+2] = m_Scene.at<cv::Vec3b>(i, k)[2] + REVISE_COLOR;
	//		item += 3;
	//	}
	//}

	//FIBITMAP *pImage = FreeImage_ConvertFromRawBits(pPixels, m_Width, m_Height, m_Width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

	//char Buffer[20];
	//_itoa_s(ScreenShotCount, Buffer, 10);
	//std::string FileName   = std::string("screenshot_") + Buffer + ".png";

	//ScreenShotCount++;

	//FreeImage_Save(FIF_PNG, pImage, FileName.c_str(), 0);

	//if (pImage)
	//{
	//	FreeImage_Unload(pImage);
	//	pImage = NULL;
	//}

	//delete pPixels;
}

//*******************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenCV::drawLine(const glm::vec2& vStart, const glm::vec2& vEnd, const glm::vec3& vColor, float vThickness)
{
	cv::Point Start = cv::Point(vStart.x, vStart.y);
	cv::Point End   = cv::Point(vEnd.x, vEnd.y);

	cv::line(m_Scene, Start, End, cv::Scalar(vColor.x, vColor.y, vColor.z), vThickness);
}

//*******************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenCV::drawRectangle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom, const glm::vec3& vColor, bool vIsFilled)
{
	float Width = abs(vRightBottom.x - vLeftTop.x);
	float Height = abs(vRightBottom.y - vLeftTop.y);
	cv::Rect Rect = cv::Rect(vLeftTop.x, vLeftTop.y, Width, Height);
	if (vIsFilled)
	{
		cv::rectangle(m_Scene, Rect, cv::Scalar(vColor.x, vColor.y, vColor.z), CV_FILLED);
	}
	else
	{
		cv::rectangle(m_Scene, Rect, cv::Scalar(vColor.x, vColor.y, vColor.z));
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenCV::clearScene()
{
	m_Scene = 0;
}

//********************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenCV::drawSphere(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor)
{
	//
}

//********************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenCV::drawCylinder(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor)
{
	//
}

//********************************************************************
//FUNCTION:
void hiveCrowdRendering::CVisualizationByOpenCV::drawCone(const glm::vec2& vPos, float vRadius, const glm::vec3& vColor)
{
	//
}
