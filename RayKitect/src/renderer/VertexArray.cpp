#include <rktpch.h>
#include "VertexArray.h"

#include <glad/glad.h>


GLenum ShaderDataTypeToGLenum(ShaderDataType type) {
	switch (type)
	{
	case ShaderDataType::Float:    return GL_FLOAT;
	case ShaderDataType::Float2:   return GL_FLOAT;
	case ShaderDataType::Float3:   return GL_FLOAT;
	case ShaderDataType::Float4:   return GL_FLOAT;
	case ShaderDataType::Mat3:     return GL_FLOAT;
	case ShaderDataType::Mat4:     return GL_FLOAT;
	case ShaderDataType::Int:      return GL_INT;
	case ShaderDataType::Int2:     return GL_INT;
	case ShaderDataType::Int3:     return GL_INT;
	case ShaderDataType::Int4:     return GL_INT;
	case ShaderDataType::Bool:     return GL_BOOL;
	}

	RKT_CORE_ERROR("Unknown ShaderDataType!");
	return 0;
}

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	Bind();
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer;
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	Bind();
	vertexBuffer->Bind();

	if (vertexBuffer->GetLayout().GetElements().size() == 0) {
		RKT_CORE_CRITICAL("Buffer has no layout!");
	}

	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout) {
		switch (element.Type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat2:
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(),
				ShaderDataTypeToGLenum(element.Type), element.Normalized, layout.GetStride(), (const void*)element.Offset);
			m_VertexBufferIndex++;
			break;
		}
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		case ShaderDataType::Bool:
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribIPointer(m_VertexBufferIndex, element.GetComponentCount(),
				ShaderDataTypeToGLenum(element.Type), layout.GetStride(), (const void*)element.Offset);
			m_VertexBufferIndex++;
			break;
		default:
			RKT_CORE_ERROR("Unknown ShaderDataType!");
		}
	}

	m_VertexBuffers.push_back(vertexBuffer);
}
