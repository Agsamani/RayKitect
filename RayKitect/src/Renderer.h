#pragma once
#include "renderer/Texture.h"
#include "Camera.h"
#include "Scene.h"
#include "Ray.h"

#include "glm/glm.hpp"

class Renderer
{
public:
	Renderer() = default;

	void Init(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera& camera);
	void OnResize(uint32_t width, uint32_t height);

	std::shared_ptr<Texture2D> GetFrameTexture() { SetTextureData(); return m_FrameTexture; }

	void ResetFrameIndex() { m_FrameIndex = 1; }

private:
	struct HitPayload { // SurfacrInteraction
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		uint32_t ObjectIndex;
		bool isTriangle;
	};

	glm::vec4 PerPixel(uint32_t x, uint32_t y);

	glm::vec3 RecTraceRay(const Ray& ray, int depth);

	HitPayload TraceRay(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray, float hitDistance, uint32_t objectIndex, bool isInside, bool isTriangle, size_t triangleIndex);
	HitPayload Miss(const Ray& ray);

	void SetTextureData();
private:
	std::shared_ptr<Texture2D> m_FrameTexture;

	const Camera* m_ActiveCamera = nullptr;
	const Scene* m_ActiveScene = nullptr;

	uint32_t m_Width, m_Height;

	uint32_t* m_ImageData;
	glm::vec4* m_AccimulationData;

	uint32_t m_FrameIndex = 1;
};

