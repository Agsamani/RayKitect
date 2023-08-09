#pragma once
#include "renderer/Texture.h"
#include "Camera.h"
#include "Scene.h"

#include "glm/glm.hpp"

class Renderer
{
public:
	Renderer() = default;

	void Init(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera& camera);
	void OnResize(uint32_t width, uint32_t height);

	std::shared_ptr<Texture2D> GetFrameTexture() { SetTextureData(); return m_FrameTexture; }

private:
	glm::vec4 PerPixel(uint32_t x, uint32_t y);
	void SetTextureData();

private:
	std::shared_ptr<Texture2D> m_FrameTexture;

	const Camera* m_ActiveCamera = nullptr;
	const Scene* m_ActiveScene = nullptr;

	uint32_t* m_ImageData;
	uint32_t m_Width, m_Height;

};

