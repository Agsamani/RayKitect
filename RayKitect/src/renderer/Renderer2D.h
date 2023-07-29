#pragma once

#include <glm/glm.hpp>
#include "camera/OrthigraphicCamera.h"
#include "Texture.h"

class Renderer2D {
public:
	static void Init();
	static void Shutdown();

	static void BeginScene(const OrthographicCamera& ortho);
	static void EndScene();

	static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
	static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec2* tecCoords, const glm::vec4& tintColor);

};