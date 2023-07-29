#include <rktpch.h>
#include "Buffer.h"

#include <glad/glad.h>

uint32_t CalculateSizeOfShaderDataType(ShaderDataType type) {
	switch (type)
	{
	case ShaderDataType::Float:    return sizeof(float);
	case ShaderDataType::Float2:   return sizeof(float) * 2;
	case ShaderDataType::Float3:   return sizeof(float) * 3;
	case ShaderDataType::Float4:   return sizeof(float) * 4;
	case ShaderDataType::Mat2:     return sizeof(float) * 2 * 2;
	case ShaderDataType::Mat3:     return sizeof(float) * 3 * 3;
	case ShaderDataType::Mat4:     return sizeof(float) * 4 * 4;
	case ShaderDataType::Int:      return sizeof(int);
	case ShaderDataType::Int2:     return sizeof(int) * 2;
	case ShaderDataType::Int3:     return sizeof(int) * 3;
	case ShaderDataType::Int4:     return sizeof(int) * 4;
	case ShaderDataType::Bool:     return sizeof(bool);
	}

	RKT_CORE_ERROR("Unknown ShaderDataType!");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
///////////////// VERTEX BUFFER //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(uint32_t size)
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(const void* data, uint32_t size)
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(const void* data, uint32_t size)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

//////////////////////////////////////////////////////////////////////////
////////////////// INDEX BUFFER //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer(const void* data, uint32_t count, uint32_t glType)
	:m_Count(count), m_GlType(glType)
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, count * (glType == GL_UNSIGNED_SHORT ? 2 : 4), data, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
