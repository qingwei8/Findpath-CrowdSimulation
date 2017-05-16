#include <iterator>
#include <iostream>
#include <fstream>
#include "Shader.h"

hiveCrowdRendering::CShader::CShader(const std::string& vVertexPath, const std::string& vFragPath)
{
	m_FileVec.push_back(SShaderFile(GL_VERTEX_SHADER, vVertexPath));
	m_FileVec.push_back(SShaderFile(GL_FRAGMENT_SHADER, vFragPath));
	__loadFromFile(m_FileVec);
}

hiveCrowdRendering::CShader::~CShader()
{
	if (this->m_ProgramId)
		glDeleteProgram(this->m_ProgramId);
}

void hiveCrowdRendering::CShader::use()
{
	glUseProgram(this->m_ProgramId);
}

//*********************************************************************
//FUNCTION:
void hiveCrowdRendering::CShader::__loadFromFile(std::vector<SShaderFile> &voShaderVec)
{
	size_t shaderCount = m_FileVec.size();
	for (size_t i=0; i<shaderCount; ++i)
	{
		std::string shaderSource;
		if (!__loadShaderSource(m_FileVec[i].filePath, shaderSource))
		{
			std::cout << "Error::Shader could not load file:" << m_FileVec[i].filePath.c_str() << std::endl; 
			return; 
		}
		m_SourceVec.push_back(shaderSource);
	}

	m_success = true;

	for (size_t i=0; i<shaderCount; ++i)
	{
		GLuint shaderId = glCreateShader(m_FileVec[i].shaderType);
		const char* shaderSource = m_SourceVec[i].c_str();
		glShaderSource(shaderId, 1, &shaderSource, NULL);
		glCompileShader(shaderId);
		GLint complieStatus;
		GLchar infoLog[512];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &complieStatus);
		if (!complieStatus)
		{
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			m_success = false;
		}
		m_ShaderObjectIdVec.push_back(shaderId);
	}

	if (m_success)
	{
		this->m_ProgramId = glCreateProgram();
		for (size_t i=0; i<shaderCount; ++i)
		{
			glAttachShader(this->m_ProgramId, m_ShaderObjectIdVec[i]);
		}
		glLinkProgram(this->m_ProgramId);
		GLint complieStatus;
		GLchar infoLog[512];
		glGetShaderiv(this->m_ProgramId, GL_COMPILE_STATUS, &complieStatus);
		if (!complieStatus)
		{
			glGetProgramInfoLog(this->m_ProgramId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}

	for (size_t i=0; i<shaderCount; ++i)
	{
		if (this->m_ProgramId != 0)
			glDetachShader(this->m_ProgramId, m_ShaderObjectIdVec[i]);
		glDeleteShader(m_ShaderObjectIdVec[i]);
	}
}

//*********************************************************************
//FUNCTION:
bool hiveCrowdRendering::CShader::__loadShaderSource(const std::string& vFilePath, std::string &voSource)
{
	voSource.clear();
	std::ifstream in_stream(vFilePath);
	if (!in_stream.is_open())
	{
		return false;
	}
	voSource.assign(std::istreambuf_iterator<char>(in_stream), 
		std::istreambuf_iterator<char>());
	return true;
}

//*********************************************************************
//FUNCTION:
void hiveCrowdRendering::CShader::disable()
{
	glUseProgram(0);
}

//*********************************************************************
//FUNCTION:
GLint hiveCrowdRendering::CShader::getUniformLocation(const std::string& vUniformName)
{
	GLuint Location = glGetUniformLocation(m_ProgramId, vUniformName.c_str());

	if (Location == GL_INVALID_VALUE)
		std::cout << "Warning! Unable to get the location of uniform " << vUniformName.c_str() << std::endl;

	return Location;
}

//*********************************************************************
//FUNCTION:
void hiveCrowdRendering::CShader::setUniformMatrix4fv(GLint vUniformLocation, const GLfloat* vUniformValue, GLboolean vIsTranspose /*= GL_FALSE*/)
{
	glUniformMatrix4fv(vUniformLocation, 1, vIsTranspose, vUniformValue);
}

//*********************************************************************
//FUNCTION:
void hiveCrowdRendering::CShader::setUniform1i(GLint vUniformLocation, GLuint vUniformValue)
{
	glUniform1i(vUniformLocation, vUniformValue);
}

//*********************************************************************
//FUNCTION:
void hiveCrowdRendering::CShader::setUniform1f(GLint vUniformLocation, GLfloat vUniformValue)
{
	glUniform1f(vUniformLocation, vUniformValue);
}

//*********************************************************************
//FUNCTION:
void hiveCrowdRendering::CShader::setUniform1fv(GLint vUniformLocation, const GLfloat* vUniformValue)
{
	glUniform1fv(vUniformLocation, 1, vUniformValue);
}

//*********************************************************************
//FUNCTION:
void hiveCrowdRendering::CShader::setUniform3fv(GLint vUniformLocation, const GLfloat* vUniformValue)
{
	glUniform3fv(vUniformLocation, 1, vUniformValue);
}

//*********************************************************************
//FUNCTION:
void hiveCrowdRendering::CShader::setUniform4fv(GLint vUniformLocation, const GLfloat* vUniformValue)
{
	glUniform4fv(vUniformLocation, 1, vUniformValue);
}
