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

	int rayCount = 1;

	for (int x = 0; x < m_Width; x++) {
		for (int y = 0; y < m_Height; y++) {
			glm::vec4 color = glm::vec4(0.0f);
			for (int i = 0; i < rayCount; i++) {
				color += PerPixel(x, y);
			}

			m_AccimulationData[x + y * m_Width] += (color)/(float)rayCount;

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

	int bounces = 10;
	for (int i = 0; i < bounces; i++) {
		Renderer::HitPayload payload = TraceRay(ray);

		if (payload.HitDistance < 0.0) {
			glm::vec3 skyColor = glm::mix(glm::vec3(0.6f, 0.9f, 1.0f), glm::vec3(0.9f), ray.Direction.y);//glm::normalize(ray.Direction) / 2.0f + 0.5f;
			light += skyColor * contribution;
			break;
		}

		int materialIndex = !payload.isTriangle ? m_ActiveScene->Spheres[payload.ObjectIndex].MaterialIndex : m_ActiveScene->Meshes[payload.ObjectIndex].MaterialIndex;
		const Material& selectedMaterial = m_ActiveScene->Materials[materialIndex];

		glm::vec3 diffuseDir = glm::normalize(payload.WorldNormal + Random::InUnitSphere());
		glm::vec3 specularDir = glm::reflect(ray.Direction, payload.WorldNormal);

		bool isSpecular = (float)(selectedMaterial.Specular >= Random::Float());

		contribution *= glm::mix(selectedMaterial.Color, selectedMaterial.SpecularColor, isSpecular);
		light += selectedMaterial.GetEmission() * contribution;

		ray.Direction = glm::mix(diffuseDir, specularDir, selectedMaterial.Smoothnes * isSpecular);
		ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
	}
	
	return glm::vec4(light, 1.0);

}

glm::vec3 Renderer::RecTraceRay(const Ray& ray, int depth)
{
	if (depth == 0) {
		return glm::vec3(0.0f);
	}

	Renderer::HitPayload payload = TraceRay(ray);

	if (payload.HitDistance < 0.0f) {
		return glm::mix(glm::vec3(0.6f, 0.9f, 1.0f), glm::vec3(0.9f), ray.Direction.y);
	}

	glm::vec3 result = glm::vec3(0.0f);
	Ray newRay;

	glm::vec3 randVec = Random::InUnitSphere();
	if (glm::dot(randVec, payload.WorldNormal) < 0.0) {
		randVec = -randVec;
	}
	newRay.Direction = glm::normalize(payload.WorldNormal + Random::InUnitSphere());

	
	newRay.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
	glm::vec3 Li = RecTraceRay(newRay, depth - 1);

	const Sphere& selectedSphere = m_ActiveScene->Spheres[payload.ObjectIndex];
	const Material& selectedMaterial = m_ActiveScene->Materials[selectedSphere.MaterialIndex];

	float PI = glm::pi<float>();
	float p = 1 / (2 * PI);
	glm::vec3 brdf = glm::vec3(1.0f) / PI;

	result = selectedMaterial.GetEmission() + (brdf * Li * glm::dot(newRay.Direction, payload.WorldNormal) / p);

	
	return result;
}

Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{
	int closestHit = -1;
	float hitDistance = std::numeric_limits<float>::max();

	bool isInside = false;
	bool isTriangle = false;// make it better

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
			closestHit = (int)i;
			isInside = false;
			continue;
		}

		float t2 = (-b + glm::sqrt(delta)) / (2.0 * a);

		if (t2 > 0.0 && t2 < hitDistance) {
			hitDistance = t2;
			closestHit = (int)i;
			isInside = true;
		}
	}

	size_t triangleIndex = 0;

	for (size_t i = 0; i < m_ActiveScene->Meshes.size(); i++) {
		const Mesh& mesh = m_ActiveScene->Meshes[i];
		size_t triangleCount = mesh.GetTriangleCount();
		for (size_t j = 0; j < triangleCount; j++) {
			Triangle triangle;
			mesh.ConstructIthTriangle(j, triangle);

			const float EPSILON = 0.000001;

			glm::vec3 edge1 = triangle.Verticies[1].Position - triangle.Verticies[0].Position;
			glm::vec3 edge2 = triangle.Verticies[2].Position - triangle.Verticies[0].Position;

			glm::vec3 h = glm::cross(ray.Direction, edge2);
			float a = glm::dot(edge1, h);

			if (glm::abs(a) < EPSILON)
				continue;

			float f = 1.0 / a;
			glm::vec3 s = ray.Origin - triangle.Verticies[0].Position;
			float u = f * glm::dot(s, h);

			if (u < 0.0f || u > 1.0f)
				continue;

			glm::vec3 q = glm::cross(s, edge1);
			float v = f * glm::dot(ray.Direction, q);

			if (v < 0.0f || u + v > 1.0f)
				continue;

			float t = f * glm::dot(edge2, q);

			if (t < EPSILON)
				continue;

			if (t < hitDistance) {
				hitDistance = t;
				closestHit = (int)i;
				triangleIndex = j;
				isTriangle = true;
			}
		}
	}

	if (closestHit < 0) {
		return Miss(ray);
	}

	return ClosestHit(ray, hitDistance, closestHit, isInside, isTriangle, triangleIndex);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, uint32_t objectIndex, bool isInside, bool isTriangle, size_t triangleIndex)
{
	Renderer::HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;
	payload.isTriangle = isTriangle;

	if (!isTriangle) {
		glm::vec3 origin = ray.Origin - m_ActiveScene->Spheres[objectIndex].Position;
		payload.WorldPosition = hitDistance * ray.Direction + origin;
		payload.WorldNormal = glm::normalize(payload.WorldPosition);
		if (isInside)
			payload.WorldNormal *= -1.0f;
		payload.WorldPosition += m_ActiveScene->Spheres[objectIndex].Position;
	}
	else {
		Triangle triangle;
		m_ActiveScene->Meshes[objectIndex].ConstructIthTriangle(triangleIndex, triangle);
		glm::vec3 edge1 = triangle.Verticies[1].Position - triangle.Verticies[0].Position;
		glm::vec3 edge2 = triangle.Verticies[2].Position - triangle.Verticies[0].Position;
		payload.WorldNormal = glm::normalize(glm::cross(edge1, edge2));
		payload.WorldPosition = hitDistance * ray.Direction + ray.Origin;
	}
	
	
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