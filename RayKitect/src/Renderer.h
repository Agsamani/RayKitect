#pragma once
#include "renderer/Texture.h"
#include "glm/glm.hpp"
class Renderer
{
public:
	Renderer() = default;

	void Init(uint32_t width, uint32_t height);

	void Render();

	std::shared_ptr<Texture2D> GetFrameTexture() { SetTextureData(); return m_FrameTexture; }

private:
	glm::vec4 OnPixel(uint32_t x, uint32_t y);
	void SetTextureData();

private:
	std::shared_ptr<Texture2D> m_FrameTexture;

	uint32_t* m_ImageData;
	uint32_t m_Width, m_Height;
};

