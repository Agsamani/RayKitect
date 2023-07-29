#pragma once

#include <string>
#include <vector>
#include "core/Base.h"


enum class ShaderDataType {
	None = 0, Bool, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat2, Mat3, Mat4
};

uint32_t CalculateSizeOfShaderDataType(ShaderDataType type);

struct LayoutElement {
	std::string Name;
	ShaderDataType Type;
	uint32_t Size;
	size_t Offset;
	bool Normalized;

	LayoutElement() = default;
	LayoutElement(ShaderDataType type, std::string name, bool normalized = false)
		:Name(name), Type(type), Size(CalculateSizeOfShaderDataType(type)), Offset(0), Normalized(normalized)
	{
	}

	uint32_t GetComponentCount() const {
		switch (Type)
		{
		case ShaderDataType::Float:    return 1;
		case ShaderDataType::Float2:   return 2;
		case ShaderDataType::Float3:   return 3;
		case ShaderDataType::Float4:   return 4;
		case ShaderDataType::Mat2:     return 2 * 2;
		case ShaderDataType::Mat3:     return 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4;
		case ShaderDataType::Int:      return 1;
		case ShaderDataType::Int2:     return 2;
		case ShaderDataType::Int3:     return 3;
		case ShaderDataType::Int4:     return 4;
		case ShaderDataType::Bool:     return 1;
		}

		RKT_CORE_ERROR("Unknown ShaderDataType!");
		return 0;
	}
};

class BufferLayout {
public:
	BufferLayout() {}
	BufferLayout(std::initializer_list<LayoutElement> elements) :
		m_Elements(elements)
	{
		CalculateElementsData();
	}

	uint32_t GetStride() const { return m_Stride; }
	const std::vector<LayoutElement>& GetElements() const { return m_Elements; }

	std::vector<LayoutElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<LayoutElement>::iterator end() { return m_Elements.end(); }
	std::vector<LayoutElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<LayoutElement>::const_iterator end() const { return m_Elements.end(); }
private:
	void CalculateElementsData() {
		size_t offset = 0;
		for (LayoutElement& element : m_Elements) {
			m_Stride += element.Size;
			element.Offset = offset;
			offset += element.Size;
		}
	}
	std::vector<LayoutElement> m_Elements;
	uint32_t m_Stride = 0;
};

class VertexBuffer {
public:
	VertexBuffer(uint32_t size);
	VertexBuffer(const void* data, uint32_t size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const void* data, uint32_t size);

	const BufferLayout& GetLayout() const { return m_Layout; }
	void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
private:
	uint32_t m_RendererID;
	BufferLayout m_Layout;
};

class IndexBuffer {
public:
	IndexBuffer(const void* data, uint32_t count, uint32_t glType = 0x1405);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	uint32_t GetCount() const { return m_Count; }
	// TODO : fix this
	uint32_t GetGlType() const { return m_GlType; }
private:
	uint32_t m_RendererID;
	uint32_t m_Count;
	uint32_t m_GlType;
};
