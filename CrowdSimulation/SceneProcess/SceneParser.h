#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "FreeImage.h"

namespace hiveSceneProcess
{
	const int g_MaxDeep       = 11;
	const int g_ExitLength    = 5;
	const int g_ThreholdPixel = 4;
	const float g_Angel       = 30;

	class CSceneParser
	{
	public:
		CSceneParser();
		~CSceneParser();

		void parseSceneImage(const std::string& vBitmapFile);
		int getSceneWidth() const {return m_Width;}
		int getSceneHeight() const {return m_Height;}
		const char* getImageData() const {return m_pImageData;}

		glm::vec2 getExit(unsigned int vIndex) const;
		unsigned int getNumExit() const {return m_ExitSet.size();}

	private:
		bool __isSameColorInSpecifiedRange(const glm::vec2& vLeftTop, const glm::vec2& vRightDown) const;
		void __recognizeExit();
		void __correctRecognizedExit();//´Ë´úÂëÓÐbug
		void __computeSampleSet(const glm::vec2& vCurPos, int vSampleRadius, int vSampleAngle, std::vector<glm::vec2>& voSamplePointSet);
		int __modifyToUniformSize(int vRealSize);
		bool __isEffectivePos(const glm::vec2& vPos);
		bool __isNecessary2ComputingSampleSet(const glm::vec2& vTargetPoint);
		bool __isSameColorInSpecifiedRange(const glm::vec2& vLeftTop, const glm::vec2& vRightDown);

		RGBQUAD __readPixelColor(const glm::vec2& vPixelPos);
		glm::vec3 __transformColorFormat(const RGBQUAD& vColor);
		std::string __changeToUniformPicture(const std::string& vBitmapFile);
		std::string __decideRepairedImageFileName(const std::string& vOldFile);

	private:
		int m_Width;
		int m_Height;
		FIBITMAP* m_pImage;
		char* m_pImageData;
		std::vector<glm::vec2> m_ExitSet;
	};
}