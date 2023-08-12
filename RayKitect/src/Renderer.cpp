#include "rktpch.h"
#include "Renderer.h"
#include "Utils.h"

void Renderer::Init(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	m_FrameTexture = std::make_shared<Texture2D>(width, height);

	m_ImageData = new uint32_t[width * height];
	m_AccimulationData = new glm::vec4[width * height];
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	m_ActiveCamera = &camera;
	m_ActiveScene = &scene;

	if (m_FrameIndex == 1) {
		memset(m_AccimulationData, 0, m_Width * m_Height * sizeof(glm::vec4));
	}

	for (int x = 0; x < m_Width; x++) {
		for (int y = 0; y < m_Height; y++) {

			m_AccimulationData[x + y * m_Width] += PerPixel(x, y);

			glm::vec4 accimulatedColor = m_AccimulationData[x + y * m_Width] / (float)m_FrameIndex;

			glm::vec4 value = glm::clamp(accimulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_Width] = Utils::CastToRGBA(value);
		}
	}

	SetTextureData();
	m_FrameIndex++;
}

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
{
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirection(x, y);

	glm::vec3 contribution(1.0f);
	glm::vec3 light(0.0f);

	int bounces = 5;
	for (int i = 0; i < bounces; i++) {
		Renderer::HitPayload payload = TraceRay(ray);

		if (payload.HitDistance < 0.0) {
			glm::vec3 skyColor = glm::vec3(0.89);//glm::normalize(ray.Direction) / 2.0f + 0.5f;
			//light += skyColor * contribution;
			break;
		}

		const Sphere& selectedSphere = m_ActiveScene->Spheres[payload.ObjectIndex];
		const Material& selectedMaterial = m_ActiveScene->Materials[selectedSphere.MaterialIndex];

		contribution *= selectedMaterial.Color;
		light += selectedMaterial.GetEmission();

		ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
		ray.Direction = glm::normalize(payload.WorldNormal + Random::InUnitSphere());
	}
	
	return glm::vec4(light, 1.0);

}

Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{
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

		if (t1 > 0.0 && t1 < hitDistance) {
			hitDistance = t1;
			closestHitSphere = (int)i;
		}
	}

	if (closestHitSphere < 0) {
		return Miss(ray);
	}

	return ClosestHit(ray, hitDistance, closestHitSphere);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, uint32_t objectIndex)
{
	Renderer::HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;

	glm::vec3 origin = ray.Origin - m_ActiveScene->Spheres[objectIndex].Position;
	payload.WorldPosition = hitDistance * ray.Direction + origin;
	payload.WorldNormal = glm::normalize(payload.WorldPosition);
	payload.WorldPosition += m_ActiveScene->Spheres[objectIndex].Position;
	
	return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
	Renderer::HitPayload payload;
	payload.HitDistance = -1.0f;
	return payload;
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

	delete m_AccimulationData;
	m_AccimulationData = new glm::vec4[width * height];

	m_FrameTexture = std::make_shared<Texture2D>(width, height);
	ResetFrameIndex();
}