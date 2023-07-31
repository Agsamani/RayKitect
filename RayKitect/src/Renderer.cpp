#include "rktpch.h"
#include "Renderer.h"
#include "Utils.h"

void Renderer::Init(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	m_FrameTexture = std::make_shared<Texture2D>(width, height);

	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render()
{
	memset(m_ImageData, 0, m_Width * m_Height);

	for (int x = 0; x < m_Width; x++) {
		for (int y = 0; y < m_Height; y++) {
			glm::vec4 value = OnPixel(x, y);
			m_ImageData[x + y * m_Width] = Utils::CastToRGBA(value);
		}
	}

	m_FrameTexture->SetData(m_ImageData, m_Width * m_Height * sizeof(uint32_t));
}

glm::vec4 Renderer::OnPixel(uint32_t x, uint32_t y)
{
	return glm::vec4((x + y * m_FrameTexture->GetWidth()) / 64.0, 0.68, 0.75, 1.0);
}
