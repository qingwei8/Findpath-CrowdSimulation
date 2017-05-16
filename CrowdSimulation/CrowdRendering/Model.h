#pragma once
#include <vector>
#include <gl\glew.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Mesh.h"

namespace hiveCrowdRendering
{
	class CShader;
	class CTexture;

	class CModel
	{
	public:
		CModel(const std::string& vFilePath);
		~CModel(void);

		void draw(CShader* vShader);

	private:
		void  __loadModel(const std::string& vFilePath);
		void  __processNode(aiNode* vNode, const aiScene* vScene);
		CMesh* __processMesh(aiMesh* vMesh, const aiScene* vScene);
//		void  __loadMeterialTextures(aiMaterial* vMaterial, aiTextureType vType, const std::string& vTypeName, std::vector<STexture>& voTextureSet);
		void  __deleteAllMesh();

	private:
		std::string			  m_Directory;
		glm::vec3			  m_Color;
//		CTexture*			  m_pTexture;
		std::vector<CMesh*>   m_MeshSet;
		std::vector<STexture> m_TextureLoaded;
	};
}