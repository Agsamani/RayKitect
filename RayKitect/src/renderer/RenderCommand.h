#pragma once
#include <glm/glm.hpp>

#include "VertexArray.h"

class RenderCommand {
public:
	static void Init();

	static void SetClearColor(const glm::vec4& color);
	static void Clear();

	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);

	static void DrawLine(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount);
	static void SetLineSmooth(bool smooth);
	static void SetLineWidth(float width);
};

