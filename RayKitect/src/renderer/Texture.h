#pragma once

#include <string>
#include "Formats.h"

struct TextureSpec {
	TextureSpec() = default;
	TextureSpec(TextureFilterFormat filterFormat, DataType dataType)
		:FilterFormat(filterFormat), Type(dataType) {}


	TextureFilterFormat FilterFormat = TextureFilterFormat::Nearest;
	DataType Type = DataType::UNSIGNED_BYTE;
};

class Texture2D
{
public:
	Texture2D(uint32_t width, uint32_t height, uint8_t channels = 4, TextureSpec spec = TextureSpec());
	Texture2D(const std::string& path, bool pixelized = false);
	~Texture2D();

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	uint8_t GetChannels() const { return m_Channels; }
	uint32_t GetRendererID() const { return m_RendererID; }

	const std::string& GetPath() const { return m_Path; }
	bool IsLoaded() const { return m_IsLoaded; }

	void SetData(void* data, uint32_t size);

	void Bind(uint32_t slot = 0);
	// TODO : generify
	void BindImage(uint32_t slot = 0);

	bool operator==(const Texture2D& other) const 
	{
		return m_RendererID == other.GetRendererID();
	}
private:
	std::string m_Path;
	uint32_t m_Width, m_Height;
	uint8_t m_Channels;
	uint32_t m_RendererID;
	bool m_IsLoaded = false;

	TextureSpec m_Spec;
};

class Texture3D
{
public:
	Texture3D(uint32_t width, uint32_t height, uint32_t depth, uint8_t channels = 4, TextureSpec spec = TextureSpec());
	~Texture3D();

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	uint32_t GetDepth() const { return m_Depth; }
	uint8_t GetChannels() const { return m_Channels; }
	uint32_t GetRendererID() const { return m_RendererID; }

	void SetData(void* data, uint32_t size);

	void Bind(uint32_t slot = 0);
	// TODO : generify
	void BindImage(uint32_t slot = 0);

	bool operator==(const Texture2D& other) const
	{
		return m_RendererID == other.GetRendererID();
	}
private:
	uint32_t m_Width, m_Height, m_Depth;
	uint8_t m_Channels;
	uint32_t m_RendererID;

	TextureSpec m_Spec;
};

class Texture1D
{
public:
	Texture1D(uint32_t width, uint8_t channels = 4, TextureSpec spec = TextureSpec());
	~Texture1D();

	uint32_t GetWidth() const { return m_Width; }
	uint8_t GetChannels() const { return m_Channels; }
	uint32_t GetRendererID() const { return m_RendererID; }

	void SetData(void* data, uint32_t size);

	void Bind(uint32_t slot = 0);
	// TODO : generify
	void BindImage(uint32_t slot = 0);

	bool operator==(const Texture2D& other) const
	{
		return m_RendererID == other.GetRendererID();
	}
private:
	uint32_t m_Width;
	uint8_t m_Channels;
	uint32_t m_RendererID;

	TextureSpec m_Spec;
};