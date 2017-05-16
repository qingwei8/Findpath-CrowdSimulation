#pragma once
#include <vector>
#include <GLM/glm.hpp>
#include <gl\glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace hiveCrowdRendering
{
	class CShader;

	struct SVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct STexture 
	{
		GLuint Id;
		std::string Type;
		aiString Path;
	};

	class CMesh
	{
	public:
		CMesh(const std::vector<SVertex>& vVertices, const std::vector<GLuint>& vIndices, const std::vector<STexture>& vTextureSet);
		~CMesh(void);
		void draw(CShader* vShader);

	private:
		void __setupMesh();

	private:
		GLuint m_VAO,m_VBO, m_EBO;
		std::vector<SVertex>  m_Vertices;
		std::vector<GLuint>   m_Indices;
		std::vector<STexture> m_TextureSet;
	};
}