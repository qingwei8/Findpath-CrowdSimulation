#include "Model.h"
//#include "Texture.h"
#include <iostream>

hiveCrowdRendering::CModel::CModel(const std::string& vFilePath)
{
//	m_pTexture = new CTexture();
	__loadModel(vFilePath);
}

hiveCrowdRendering::CModel::~CModel(void)
{
//	delete m_pTexture;
	__deleteAllMesh();
}

//******************************************************************************
//FUNCTION:
void hiveCrowdRendering::CModel::draw(CShader* vShader)
{
	for (GLuint i=0; i<m_MeshSet.size(); ++i)
		m_MeshSet[i]->draw(vShader);
}

//******************************************************************************
//FUNCTION:
void hiveCrowdRendering::CModel::__loadModel(const std::string& vFilePath)
{
	Assimp::Importer import;
	const aiScene* pScene = import.ReadFile(vFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!pScene || pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	m_Directory = vFilePath;
	__processNode(pScene->mRootNode, pScene);
}

//******************************************************************************
//FUNCTION:
void hiveCrowdRendering::CModel::__processNode(aiNode* vNode, const aiScene* vScene)
{
	for (GLuint i=0; i<vNode->mNumMeshes; ++i)
	{
		aiMesh* pMesh = vScene->mMeshes[vNode->mMeshes[i]];
		m_MeshSet.push_back(__processMesh(pMesh, vScene));
	}
	for (GLuint i=0; i<vNode->mNumChildren; ++i)
		__processNode(vNode->mChildren[i], vScene);
}

//******************************************************************************
//FUNCTION:
hiveCrowdRendering::CMesh* hiveCrowdRendering::CModel::__processMesh(aiMesh* vMesh, const aiScene* vScene)
{
	std::vector<SVertex>  Vertices;
	std::vector<GLuint>   Indices;
	std::vector<STexture> TextureSet;

	for (GLuint i=0; i<vMesh->mNumVertices; ++i)
	{
		SVertex Vertex;
		glm::vec3 CurVector3;
		CurVector3.x = vMesh->mVertices[i].x;
		CurVector3.y = vMesh->mVertices[i].y;
		CurVector3.z = vMesh->mVertices[i].z;
		Vertex.Position = CurVector3;

		CurVector3.x = vMesh->mNormals[i].x;
		CurVector3.y = vMesh->mNormals[i].y;
		CurVector3.z = vMesh->mNormals[i].z;
		Vertex.Normal = CurVector3;

		if (vMesh->mTextureCoords[0])
		{
			glm::vec2 CurVector2;
			CurVector2.x = vMesh->mTextureCoords[0][i].x;
			CurVector2.y = vMesh->mTextureCoords[0][i].y;
			Vertex.TexCoords = CurVector2;
		}
		else
			Vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		Vertices.push_back(Vertex);
	}
	
	for (GLuint i=0; i<vMesh->mNumFaces; ++i)
	{
		aiFace Face = vMesh->mFaces[i];
		for (GLuint k=0; k<Face.mNumIndices; ++k)
			Indices.push_back(Face.mIndices[k]);
	}
	/*if (vMesh->mMaterialIndex >= 0)
	{
	aiMaterial* pMaterial = vScene->mMaterials[vMesh->mMaterialIndex];

	std::vector<STexture> DiffuseMaps;
	__loadMeterialTextures(pMaterial, aiTextureType_DIFFUSE, "texture_diffuse", DiffuseMaps);
	TextureSet.insert(TextureSet.end(), DiffuseMaps.begin(), DiffuseMaps.end());

	std::vector<STexture> SpecularMaps;
	__loadMeterialTextures(pMaterial, aiTextureType_SPECULAR, "texture_specular", SpecularMaps);
	TextureSet.insert(TextureSet.end(), SpecularMaps.begin(), SpecularMaps.end());
	}*/
	return new CMesh(Vertices, Indices, TextureSet);
}

//******************************************************************************
//FUNCTION:
//void hiveCrowdRendering::CModel::__loadMeterialTextures(aiMaterial* vMaterial, aiTextureType vType, const std::string& vTypeName, std::vector<STexture>& voTextureSet)
//{
//	for (GLuint i=0; i<vMaterial->GetTextureCount(vType); ++i)
//	{
//		aiString AiString;
//		vMaterial->GetTexture(vType, i , &AiString);
//		GLboolean IsSkip = false;
//		for (GLuint k=0; k<m_TextureLoaded.size(); ++k)
//		{
//			if (m_TextureLoaded[k].Path == AiString)
//			{
//				voTextureSet.push_back(m_TextureLoaded[k]);
//				IsSkip = true;
//				break;
//			}
//		}
//		if (!IsSkip)
//		{
//			STexture CurTexture;
//			if (m_pTexture->addTexture(m_Directory + "\\" + AiString.C_Str(), CurTexture.Id))
//			{
//				CurTexture.Type = vTypeName;
//				CurTexture.Path = AiString;
//				voTextureSet.push_back(CurTexture);
//				m_TextureLoaded.push_back(CurTexture);
//			}
//		}
//	}
//}

//******************************************************************************
//FUNCTION:
void hiveCrowdRendering::CModel::__deleteAllMesh()
{
	for (unsigned int i=0; i<m_MeshSet.size(); ++i)
		delete m_MeshSet[i];
}