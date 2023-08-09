#include "rktpch.h"
#include "Renderer.h"
#include "Utils.h"

#include "Ray.h"

void Renderer::Init(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	m_FrameTexture = std::make_shared<Texture2D>(width, height);

	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	m_ActiveCamera = &camera;
	m_ActiveScene = &scene;

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
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirection(x, y);

	int closestHitSphere = -1;
	float hitDistance = std::numeric_limits<float>::max();

	for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++) {
		const Sphere& sphere = m_ActiveScene->Spheres[i];
		glm::vec3 origin = ray.Origin - sphere.Position;

		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2.0 * glm::dot(ray.Direction, origin);
		float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

		float delta = b * b - 4 * a * c;

		if (delta < 0.0f) {
			continue;
		}

		float t1 = (-b - glm::sqrt(delta)) / (2.0 * a);

		if (t1 < hitDistance) {
			hitDistance = t1;
			closestHitSphere = (int)i;
		}
	}

	if (closestHitSphere < 0) {
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	glm::vec3 origin = ray.Origin - m_ActiveScene->Spheres[closestHitSphere].Position;
	glm::vec3 intersection = (hitDistance) * ray.Direction + origin;

	glm::vec3 normal = glm::normalize(intersection);
	glm::vec3 lightDir = glm::vec3(1.0, 2.0, 3.0);

	glm::vec4 result = glm::vec4(m_ActiveScene->Spheres[closestHitSphere].Color, 1.0) * glm::max(0.0f, glm::dot(normalize(lightDir), normal));
	result.w = 1.0f;
	return result;
}

void Renderer::SetTextureData()
{
	m_FrameTexture->SetData(m_ImageData, m_Width * m_Height * sizeof(uint32_t));
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;
	delete m_ImageData;
	m_ImageData = new uint32_t[width * height];
	m_FrameTexture = std::make_shared<Texture2D>(width, height);
}