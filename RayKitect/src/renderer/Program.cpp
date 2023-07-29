#include "rktpch.h"
#include "Program.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>


Program::Program()
{
	m_RendererID = glCreateProgram();
}

Program::~Program()
{
	glDeleteProgram(m_RendererID);
	// TODO : Maybe detach shaders?
}

void Program::AttachShader(const Shader& shader) const
{
	glAttachShader(m_RendererID, shader.GetShaderID());
}

void Program::LinkProgram() const
{
	glLinkProgram(m_RendererID);
}

void Program::UseProgram() const
{
	glUseProgram(m_RendererID);
}

void Program::UploadUniformInt(const std::string& name, int value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(location, value);
}

void Program::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1iv(location, count, values);
}

void Program::UploadUniformFloat(const std::string& name, float value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1f(location, value);
}

void Program::UploadUniformFloat2(const std::string& name, const glm::vec2& value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void Program::UploadUniformFloat3(const std::string& name, const glm::vec3& value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void Program::UploadUniformFloat4(const std::string& name, const glm::vec4& value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Program::UploadUniformMat2(const std::string& name, const glm::mat2& matrix) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Program::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Program::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}