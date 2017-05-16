#include "Mesh.h"
#include "Shader.h"
#include <sstream>

hiveCrowdRendering::CMesh::CMesh(const std::vector<SVertex>& vVertices, const std::vector<GLuint>& vIndices, const std::vector<STexture>& vTextureSet)
{
	m_Vertices   = vVertices;
	m_Indices    = vIndices;
	m_TextureSet = vTextureSet;
	__setupMesh();
}


hiveCrowdRendering::CMesh::~CMesh(void)
{
}

//******************************************************************************
//FUNCTION:
void hiveCrowdRendering::CMesh::__setupMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(SVertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)offsetof(SVertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)offsetof(SVertex, TexCoords));

	glBindVertexArray(0);
}

//******************************************************************************
//FUNCTION:
void hiveCrowdRendering::CMesh::draw(CShader* vShader)
{
	GLuint DiffuseNumber  = 1;
	GLuint SpecularNumber = 1;
	for (GLuint i=0; i < m_TextureSet.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::stringstream ss;
		std::string Number;
		std::string Name = m_TextureSet[i].Type;
		if (Name == "texture_diffuse")
			ss << DiffuseNumber++;
		else if (Name == "texture_specular")
			ss << SpecularNumber++;
		Number = ss.str();

		glUniform1f(glGetUniformLocation(vShader->m_ProgramId, ("material." + Name + Number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, m_TextureSet[i].Id);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}