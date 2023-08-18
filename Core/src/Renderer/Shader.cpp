#include "pch.h"
#include "Shader.h"

#include <fstream>
#include <sstream>
#include <glad/glad.h>

Shader::Shader(const std::string& shaderPath)
{
	m_RendererID = glCreateProgram();
	CompileShader(shaderPath, m_RendererID);
	s_Shaders.push_back({ shaderPath, m_RendererID });
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::UnBind() const
{
	glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void Shader::SetIntArray(const std::string& name, int* value) const
{
	glUniform1iv(glGetUniformLocation(m_RendererID, name.c_str()), sizeof(value) / 4, value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
}

void Shader::SetFloat(const std::string& name, const float& value) const
{
	glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void Shader::CompileShader(const std::string& shaderPath, const uint32_t RendererID)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string shaderCode;
	std::ifstream ShaderFile;
	// ensure ifstream objects can throw exceptions:
	ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		ShaderFile.open(shaderPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << ShaderFile.rdbuf();
		// close file handlers
		ShaderFile.close();
		// convert stream into string
		shaderCode = vShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}

	std::string vShaderCode;
	std::string fShaderCode;

	std::string test1,test2;

	for(int i=0;i<shaderCode.length();i++)
	{
		if (test1 == "$vertexshader")
		{
			if (shaderCode[i] == '$')
			{
				test1.clear();
				test2.clear();
			}
			else
			{
				vShaderCode.push_back(shaderCode[i]);
			}
		}
		else
		{
			test1.push_back(shaderCode[i]);
		}

		if(test2=="$fragmentshader")
		{
			fShaderCode.push_back(shaderCode[i]);
		}
		else
		{
			test2.push_back(shaderCode[i]);
		}
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* source = (const GLchar*)vShaderCode.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled==GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);


		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(vertexShader);

		ERROR("VertexShader Compilation Error: " << infoLog.data());
		return;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	source = (const GLchar*)fShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled==GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);


		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		ERROR("FragmentShader Compilation Error: " << infoLog.data());
		return;
	}

	GLuint program = RendererID;

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if(isLinked==GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

}

void Shader::ReCompileShaders()
{
	for (auto shader : s_Shaders)
	{
		CompileShader(shader.ShaderPath, shader.RendererID);
	}
}
