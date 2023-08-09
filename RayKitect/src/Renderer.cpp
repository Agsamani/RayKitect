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

void Renderer::Render(const Camera& camera)
{
	m_SceneCamera = &camera;

	memset(m_ImageData, 0, m_Width * m_Height);

	for (int x = 0; x < m_Width; x++) {
		for (int y = 0; y < m_Height; y++) {
			glm::vec4 value = glm::clamp(PerPixel(x, y), glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_Width] = Utils::CastToRGBA(value);
		}
	}

	SetTextureData();
}

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
{

	glm::vec3 cameraPos = m_SceneCamera->GetPosition();
	glm::vec3 spherePos = glm::vec3(0.0, 0.0, 0.0);
	float r = 0.6;

	glm::vec3 ray = m_SceneCamera->GetRayDirection(x, y);

	float a = glm::dot(ray, ray);
	float b = 2.0 * glm::dot(ray, cameraPos);
	float c = glm::dot(cameraPos, cameraPos) - r * r;

	float delta = b * b - 4 * a * c;

	float t1 = (- b - glm::sqrt(delta)) / (2.0 * a);

	glm::vec3 intersection1 = (t1) * ray + cameraPos;

	glm::vec3 normal = glm::normalize(intersection1 - spherePos);
	glm::vec3 lightDir = glm::vec3(1.0, 2.0, 3.0);

	if (delta >= 0) {
		return glm::vec4(0.35 , 0.78, 0.95, 1.0) * glm::max(0.0f, glm::dot(-normalize(lightDir), normal));
	}
	else {
		return glm::vec4(0.0f);//glm::vec4((x + y * m_FrameTexture->GetWidth()) / 640000.0, 0.68, 0.75, 1.0);
	}

}

void Renderer::SetTextureData()
{
	m_FrameTexture->SetData(m_ImageData, m_Width * m_Height * sizeof(uint32_t));
}
