#include "rktpch.h"
#include "Texture.h"

#include <glad/glad.h>
#include "core/Base.h"

#include "stb_image.h"

// TODO : use ither formats like rgba32f
GLenum GetGLDataFormat(uint8_t channels) {
	switch (channels) {
	case 1: return GL_RED;
	case 2: return GL_RG;
	case 3: return GL_RGB;
	case 4: return GL_RGBA;
	}

	RKT_CORE_ERROR("Uknown channel count : {0}", channels);
	return -1;
}

GLenum GetGLInternalFormat(uint8_t channels, DataType dataType) {
	switch (dataType) {
	case DataType::UNSIGNED_BYTE: {
		switch (channels) {
		case 1: return GL_R8;
		case 2: return GL_RG8;
		case 3: return GL_RGB8;
		case 4: return GL_RGBA8;
		}
	}
	case DataType::FLOAT: {
		switch (channels) { // TODO : maybe 16f later
		case 1: return GL_R32F;
		case 2: return GL_RG32F;
		case 3: return GL_RGB32F;
		case 4: return GL_RGBA32F;
		}
	}
	}

	RKT_CORE_ERROR("Uknown channel count : {0} or data type", channels);
	return -1;
}

GLenum GetGLDataType(DataType dataType) {
	switch (dataType) {
	case DataType::UNSIGNED_BYTE:   return GL_UNSIGNED_BYTE;
	case DataType::BYTE:			return GL_BYTE;
	case DataType::UNSIGNED_SHORT:  return GL_UNSIGNED_SHORT;
	case DataType::SHORT:			return GL_SHORT;
	case DataType::UNSIGNED_INT:    return GL_UNSIGNED_INT;
	case DataType::INT:			    return GL_INT;
	case DataType::FLOAT:			return GL_FLOAT;

	}
	RKT_CORE_ERROR("Uknown data type");
	return -1;
}

int GetGLDataTypeSize(DataType dataType) {
	switch (dataType) {
	case DataType::UNSIGNED_BYTE:   return 1;
	case DataType::BYTE:			return 1;
	case DataType::UNSIGNED_SHORT:  return 2;
	case DataType::SHORT:			return 2;
	case DataType::UNSIGNED_INT:    return 4;
	case DataType::INT:			    return 4;
	case DataType::FLOAT:			return 4;

	}
	RKT_CORE_ERROR("Uknown data type");
	return -1;
}

//////////////////////////////////////////////////////////////////////////
///////////// Texture 2d /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	
Texture2D::Texture2D(uint32_t width, uint32_t height, uint8_t channels /*= 4*/, TextureSpec spec)
	:m_Width(width), m_Height(height), m_Channels(channels), m_Spec(spec)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, GetGLInternalFormat(m_Channels, m_Spec.Type), m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_Spec.FilterFormat == TextureFilterFormat::Nearest ? GL_NEAREST : GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, m_Spec.FilterFormat == TextureFilterFormat::Nearest ? GL_NEAREST : GL_LINEAR);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture2D::Texture2D(const std::string& path, bool pixelized /*= false*/)
	:m_Path(path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = nullptr;
	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	m_Width = width;
	m_Height = height;
	m_Channels = channels;
	if (pixelized) {
		m_Spec = TextureSpec(TextureFilterFormat::Nearest, DataType::UNSIGNED_BYTE);
	}
	else {
		m_Spec = TextureSpec(TextureFilterFormat::Linear, DataType::UNSIGNED_BYTE);
	}

	if (data) {
		m_IsLoaded = true;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GetGLInternalFormat(channels, m_Spec.Type), width, height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, pixelized ? GL_NEAREST : GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, pixelized ? GL_NEAREST : GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GetGLDataFormat(channels), GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	else {
		RKT_CORE_ERROR("Failed to load texture : {0}", m_Path);
	}
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture2D::SetData(void* data, uint32_t size)
{
	if (size != m_Width * m_Height * m_Channels * GetGLDataTypeSize(m_Spec.Type)) {
		RKT_CORE_ERROR("Data is not the entire texture!");
	}
	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GetGLDataFormat(m_Channels), GetGLDataType(m_Spec.Type), data);
}

void Texture2D::Bind(uint32_t slot /*= 0*/)
{
	glBindTextureUnit(slot, m_RendererID);
}

void Texture2D::BindImage(uint32_t slot /*= 0*/)
{
	glBindImageTexture(slot, m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GetGLInternalFormat(m_Channels, m_Spec.Type));
}

//////////////////////////////////////////////////////////////////////////
///////////// Texture 3d /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Texture3D::Texture3D(uint32_t width, uint32_t height, uint32_t depth, uint8_t channels /*= 4*/, TextureSpec spec)
	:m_Width(width), m_Height(height), m_Depth(depth), m_Channels(channels), m_Spec(spec)
{
	glCreateTextures(GL_TEXTURE_3D, 1, &m_RendererID);
	glTextureStorage3D(m_RendererID, 1, GetGLInternalFormat(m_Channels, m_Spec.Type), m_Width, m_Height, m_Depth);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_Spec.FilterFormat == TextureFilterFormat::Nearest ? GL_NEAREST : GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, m_Spec.FilterFormat == TextureFilterFormat::Nearest ? GL_NEAREST : GL_LINEAR);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture3D::~Texture3D()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture3D::SetData(void* data, uint32_t size)
{
	if (size != m_Width * m_Height * m_Depth * m_Channels * GetGLDataTypeSize(m_Spec.Type)) {
		RKT_CORE_ERROR("Data is not the entire texture!");
	}
	glTextureSubImage3D(m_RendererID, 0, 0, 0, 0, m_Width, m_Height, m_Depth, GetGLDataFormat(m_Channels), GetGLDataType(m_Spec.Type), data);
}

void Texture3D::Bind(uint32_t slot /*= 0*/)
{
	glBindTextureUnit(slot, m_RendererID);
}

void Texture3D::BindImage(uint32_t slot /*= 0*/)
{
	glBindImageTexture(slot, m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GetGLInternalFormat(m_Channels, m_Spec.Type));
}

//////////////////////////////////////////////////////////////////////////
///////////// Texture 1d /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Texture1D::Texture1D(uint32_t width, uint8_t channels /*= 4*/, TextureSpec spec)
	:m_Width(width), m_Channels(channels), m_Spec(spec)
{
	glCreateTextures(GL_TEXTURE_1D, 1, &m_RendererID);
	glTextureStorage1D(m_RendererID, 1, GetGLInternalFormat(m_Channels, m_Spec.Type), m_Width);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_Spec.FilterFormat == TextureFilterFormat::Nearest ? GL_NEAREST : GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, m_Spec.FilterFormat == TextureFilterFormat::Nearest ? GL_NEAREST : GL_LINEAR);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture1D::~Texture1D()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture1D::SetData(void* data, uint32_t size)
{
	if (size != m_Width * m_Channels * GetGLDataTypeSize(m_Spec.Type)) {
		RKT_CORE_ERROR("Data is not the entire texture!");
	}
	glTextureSubImage1D(m_RendererID, 0, 0, m_Width, GetGLDataFormat(m_Channels), GetGLDataType(m_Spec.Type), data);
}

void Texture1D::Bind(uint32_t slot /*= 0*/)
{
	glBindTextureUnit(slot, m_RendererID);
}

void Texture1D::BindImage(uint32_t slot /*= 0*/)
{
	glBindImageTexture(slot, m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GetGLInternalFormat(m_Channels, m_Spec.Type));
}
