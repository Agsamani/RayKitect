#include <rktpch.h>
#include "Shader.h"

#include <glad/glad.h>

GLenum ShaderTypeToOpenGlType(ShaderType type) {
	switch (type)
	{
	case ShaderType::Vertex:			return GL_VERTEX_SHADER;
	case ShaderType::TessControl:		return GL_TESS_CONTROL_SHADER;
	case ShaderType::TessEvaluate:	return GL_TESS_EVALUATION_SHADER;
	case ShaderType::Geometry:		return GL_GEOMETRY_SHADER;
	case ShaderType::Fragment:		return GL_FRAGMENT_SHADER;
	case ShaderType::Compute:			return GL_COMPUTE_SHADER;
	}

	RKT_CORE_ERROR("Unknown ShaderType!");
	return 0;
}

Shader::Shader(std::string path, ShaderType type)
	:m_Type(type)
{
	m_RendererID = glCreateShader(ShaderTypeToOpenGlType(m_Type));

	// TODO : optimize this
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string str = buffer.str();
	const char* shaderData = str.c_str();
	glShaderSource(m_RendererID, 1, &shaderData, NULL);
	glCompileShader(m_RendererID);

	GLint isCompiled = 0;
	glGetShaderiv(m_RendererID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog;
		errorLog.reserve(maxLength);
		glGetShaderInfoLog(m_RendererID, maxLength, &maxLength, errorLog.data());

		RKT_CORE_CRITICAL("Shader compilation: {0}", errorLog.c_str());

		//glDeleteShader(shader); // Don't leak the shader.
	}
}

Shader::~Shader()
{
	glDeleteShader(m_RendererID);
}