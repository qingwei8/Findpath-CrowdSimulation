#pragma once
#include <gl/glew.h>
#include <vector>

namespace hiveCrowdRendering
{
	struct SShaderFile
	{
		GLenum shaderType;
		const std::string filePath;
		SShaderFile(GLenum vType, const std::string& vPath) : shaderType(vType), filePath(vPath) {}
	};

	class CShader
	{
	public:
		CShader(const std::string& vVertexPath, const std::string& vFragPath);
		~CShader();

		void use();
		void disable();
		GLint getUniformLocation(const std::string& vUniformName);
		void setUniformMatrix4fv(GLint vUniformLocation, const GLfloat* vUniformValue, GLboolean vIsTranspose = GL_FALSE);
		void setUniform1i(GLint vUniformLocation, GLuint vUniformValue);
		void setUniform1f(GLint vUniformLocation, GLfloat vUniformValue);
		void setUniform1fv(GLint vUniformLocation, const GLfloat* vUniformValue);
		void setUniform3fv(GLint vUniformLocation, const GLfloat* vUniformValue);
		void setUniform4fv(GLint vUniformLocation, const GLfloat* vUniformValue);

	public:
		GLuint m_ProgramId;

	private:
		void __loadFromFile(std::vector<SShaderFile> &voShaderVec);
		bool __loadShaderSource(const std::string& vFilePath, std::string& voSource);

	private:
		bool m_success;
		std::vector<SShaderFile> m_FileVec;
		std::vector<GLuint>		 m_ShaderObjectIdVec;
		std::string				 m_VertexSource, m_FragSource;
		std::vector<std::string> m_SourceVec;
	};
}