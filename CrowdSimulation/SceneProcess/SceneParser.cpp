#include "SceneParser.h"
#include <math.h>
#include <tchar.h>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace hiveSceneProcess;

CSceneParser::CSceneParser()
{
}

CSceneParser::~CSceneParser()
{
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CSceneParser::parseSceneImage(const std::string& vBitmapFile)
{
	_ASSERT(!vBitmapFile.empty());

	std::string BitmapFile = __changeToUniformPicture(vBitmapFile);
	_ASSERT(!BitmapFile.empty());
	FreeImage_Initialise(TRUE);

	FREE_IMAGE_FORMAT Fif = FreeImage_GetFIFFromFilename(BitmapFile.c_str());
	_ASSERT(Fif != FIF_UNKNOWN);
	m_pImage = FreeImage_Load(FIF_BMP, BitmapFile.c_str(), BMP_DEFAULT);
	_ASSERT(m_pImage);

	m_pImage = FreeImage_ConvertTo8Bits(m_pImage);
	m_Width  = FreeImage_GetWidth(m_pImage);
	m_Height = FreeImage_GetHeight(m_pImage);
	m_pImageData = (char*)malloc(sizeof(char) * m_Width * m_Height);
	memcpy(m_pImageData, FreeImage_GetBits(m_pImage), m_Width * m_Height);

	__recognizeExit();

	FreeImage_Unload(m_pImage);
	FreeImage_DeInitialise();
}

//********************************************************************
//FUNCTION:
std::string hiveSceneProcess::CSceneParser::__changeToUniformPicture(const std::string& vBitmapFile)
{
	cv::Mat SourceImage = cv::imread(vBitmapFile, 0);
	int WidthIncrease = __modifyToUniformSize(SourceImage.cols);
	int HeightIncrease = __modifyToUniformSize(SourceImage.rows);

	if(WidthIncrease ==0 && HeightIncrease == 0)
	{
		return vBitmapFile;
	}	
	else
	{
		std::string NewFileName = __decideRepairedImageFileName(vBitmapFile);

		FreeImage_Initialise(TRUE); 
		FIBITMAP* pLoadBmp = FreeImage_Load(FIF_BMP, vBitmapFile.c_str(), BMP_DEFAULT);
		int Width = FreeImage_GetWidth(pLoadBmp);
		int Height = FreeImage_GetHeight(pLoadBmp);
		FIBITMAP* pAllocateImage = FreeImage_Allocate(Width+WidthIncrease, Height+HeightIncrease, 8, 0, 0, 0);
		for (int i=0; i<Height; ++i)
		{
			BYTE* SourceBits = FreeImage_GetScanLine(pLoadBmp, i);
			BYTE* TargetBits = FreeImage_GetScanLine(pAllocateImage, i);
			for (int k=0; k<Width; ++k)
			{
				TargetBits[k] = SourceBits[k];
			}
		}
		if(pAllocateImage)
		{
			
			FreeImage_Save(FIF_BMP, pAllocateImage, NewFileName.c_str(), BMP_DEFAULT);
		}

		FreeImage_Unload(pLoadBmp);
		FreeImage_Unload(pAllocateImage);
		FreeImage_DeInitialise();

		return NewFileName;
	}
}

//********************************************************************
//FUNCTION:
std::string hiveSceneProcess::CSceneParser::__decideRepairedImageFileName(const std::string& vOldFile)
{
	_ASSERT(!vOldFile.empty());

	std::vector<std::string> SplitResultStr;
	boost::split(SplitResultStr, vOldFile, boost::is_any_of(_T("/")));

 	//std::string PureShaderText;
 	//std::vector<boost::iterator_range<std::string::iterator> > VecString;
 	//boost::split(VecString, PureShaderText, boost::is_any_of("; \n"),  boost::token_compress_on);

	std::string ImagePath;
	if (SplitResultStr.size() > 1)
	{
		for (unsigned int i=0; i<SplitResultStr.size() - 1; ++i)
		{
			ImagePath += (SplitResultStr[i] + '/');
		}
	}

	std::vector<std::string> FileNameSplitResultStr;
	boost::split(FileNameSplitResultStr, SplitResultStr[SplitResultStr.size() - 1], boost::is_any_of(_T(".")));
	_ASSERT(FileNameSplitResultStr.size() == 2);

	std::string ImageName = FileNameSplitResultStr[0] + "_New.";
	ImagePath += ImageName;
	ImagePath += FileNameSplitResultStr[1];

	return ImagePath;
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CSceneParser::__recognizeExit()
{
	for (int i=0; i<m_Width; ++i)
	{
		for (int k=0; k<m_Height; ++k)
		{
			RGBQUAD CurrentPixelColor = __readPixelColor(glm::vec2(i, k));
			glm::vec3 CurrentPixelParsedColor = __transformColorFormat(CurrentPixelColor);
			if (CurrentPixelParsedColor != glm::vec3(255, 255, 255) && CurrentPixelParsedColor != glm::vec3(0, 0, 0))
			{
				//compute sample point set
				if (__isNecessary2ComputingSampleSet(glm::vec2(i, k)))
				{
					std::vector<glm::vec2> SamplePointSet;
					__computeSampleSet(glm::vec2(i, k), g_ExitLength, g_Angel, SamplePointSet);
					int NumEffectiveSamplePoint = 0;
					for (auto& SamplePoint : SamplePointSet)
					{
						RGBQUAD SamplePointColor = __readPixelColor(SamplePoint);
						glm::vec3 ParsedColor = __transformColorFormat(SamplePointColor);
						if (ParsedColor == CurrentPixelParsedColor)
						{
							++NumEffectiveSamplePoint;
						}
					}
					if (NumEffectiveSamplePoint == g_ThreholdPixel)
					{
						m_ExitSet.push_back(glm::vec2(i, k));
					}
				}
			}
		}
	}

	//__correctRecognizedExit();
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CSceneParser::__correctRecognizedExit()
{
	for (unsigned int i=0; i<m_ExitSet.size(); ++i)
	{
		for (unsigned int k=0; k<m_ExitSet.size(); ++k)
		{
			if (k == i)
			{
				continue;
			}
			if (__isSameColorInSpecifiedRange(m_ExitSet[i], m_ExitSet[k]))
			{
				m_ExitSet[i].x < m_ExitSet[k].x ? m_ExitSet.erase(m_ExitSet.begin() + k) : m_ExitSet.erase(m_ExitSet.begin() + i);
			}
		}
	}
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CSceneParser::__isSameColorInSpecifiedRange(const glm::vec2& vLeftTop, const glm::vec2& vRightDown)
{
	glm::vec2 XRange, YRange;
	XRange.x = std::min(vLeftTop.x, vRightDown.x);
	XRange.y = std::max(vLeftTop.x, vRightDown.x);
	YRange.x = std::min(vLeftTop.y, vRightDown.y);
	YRange.y = std::max(vLeftTop.y, vRightDown.y);

	for (int i=YRange.x; i<YRange.y - 1; ++i)
	{
		for (int k=XRange.x; k<XRange.y; ++k)
		{
			RGBQUAD UpLinePoint     = __readPixelColor(glm::vec2(i, k));
			glm::vec3 UpLineColor   = __transformColorFormat(UpLinePoint);
			RGBQUAD DownLinePoint   = __readPixelColor(glm::vec2(i+1, k));
			glm::vec3 DownLineColor = __transformColorFormat(DownLinePoint);
			if (UpLineColor != DownLineColor)
			{
				return false;
			}
		}
	}
	return true;
}

//********************************************************************
//FUNCTION:
RGBQUAD hiveSceneProcess::CSceneParser::__readPixelColor(const glm::vec2& vPixelPos)
{
	_ASSERT(m_pImage);

	int ColorDepth = FreeImage_GetBPP(m_pImage);
	RGBQUAD Color;
	if (ColorDepth==16 || ColorDepth==24 || ColorDepth==32)
	{
		if (!FreeImage_GetPixelColor(m_pImage, vPixelPos.x, vPixelPos.y, &Color))
			printf("read fail.\n");
	}
	else if(ColorDepth==1 || ColorDepth==4 || ColorDepth==8)
	{
		BYTE Index;
		if (!FreeImage_GetPixelIndex(m_pImage, vPixelPos.x, vPixelPos.y, &Index))
			printf("read fail.\n");
		else
		{
			RGBQUAD* pPalette = (RGBQUAD*)FreeImage_GetPalette(m_pImage);
			Color = pPalette[Index];
		}
	}

	return Color;
}

//********************************************************************
//FUNCTION:
glm::vec3 hiveSceneProcess::CSceneParser::__transformColorFormat(const RGBQUAD& vColor)
{
	return glm::vec3(vColor.rgbBlue, vColor.rgbGreen, vColor.rgbRed);
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CSceneParser::__isNecessary2ComputingSampleSet(const glm::vec2& vTargetPoint)
{
	_ASSERT(m_pImage);
	_ASSERT(__isEffectivePos(vTargetPoint));

	glm::vec2 LeftPoint = glm::vec2(vTargetPoint.x-1, vTargetPoint.y);
	glm::vec2 TopPoint  = glm::vec2(vTargetPoint.x, vTargetPoint.y-1);
	RGBQUAD TargetPointColor = __readPixelColor(vTargetPoint);
	glm::vec3 TargetPointParsedColor = __transformColorFormat(TargetPointColor);

	if (__isEffectivePos(LeftPoint))
	{
		RGBQUAD LeftPointColor = __readPixelColor(LeftPoint);
		glm::vec3 LeftPointParsedColor = __transformColorFormat(LeftPointColor);
		if (TargetPointParsedColor == LeftPointParsedColor)
		{
			return false;
		}
	}

	if (__isEffectivePos(TopPoint))
	{
		RGBQUAD TopPointColor = __readPixelColor(TopPoint);
		glm::vec3 TopPointParsedColor = __transformColorFormat(TopPointColor);
		if (TargetPointParsedColor == TopPointParsedColor)
		{
			return false;
		}
	}

	return true;
}

//********************************************************************
//FUNCTION:
bool hiveSceneProcess::CSceneParser::__isEffectivePos(const glm::vec2& vPos)
{
	return vPos.x >=0 && vPos.x < m_Width && vPos.y >=0 && vPos.y < m_Height;
}

//********************************************************************
//FUNCTION:
int hiveSceneProcess::CSceneParser::__modifyToUniformSize(int vRealSize)
{
	int TempDeep;
	for (int i=0; i<=g_MaxDeep; i++)
	{
		if (vRealSize <= std::pow(2, i))
		{
			TempDeep = i;
			break;
		}
	}
	return ((int)std::pow(2, TempDeep) - vRealSize);
}

//********************************************************************
//FUNCTION:
void hiveSceneProcess::CSceneParser::__computeSampleSet(const glm::vec2& vCurPos, int vSampleRadius, int vSampleAngle, std::vector<glm::vec2>& voSamplePointSet)
{
	for (double Angle = 0.0; Angle <= 90.0; Angle+=vSampleAngle)
	{
		double Radian = Angle * M_PI / 180.0f;
		voSamplePointSet.push_back(vCurPos + glm::vec2(vSampleRadius*std::cos(Radian), vSampleRadius*std::sin(Radian)));
	}
}

//********************************************************************
//FUNCTION:
glm::vec2 hiveSceneProcess::CSceneParser::getExit(unsigned int vIndex) const
{
	_ASSERT(vIndex < m_ExitSet.size());
	return m_ExitSet[vIndex];
}