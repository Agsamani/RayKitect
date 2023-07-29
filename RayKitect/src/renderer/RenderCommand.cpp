#include "rktpch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

#include "core/Base.h"

void OpenGLMessageCallback(
	unsigned source,
	unsigned type,
	unsigned id,
	unsigned severity,
	int length,
	const char* message,
	const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         RKT_CORE_CRITICAL(message); return;
	case GL_DEBUG_SEVERITY_MEDIUM:       RKT_CORE_ERROR(message); return;
	case GL_DEBUG_SEVERITY_LOW:          RKT_CORE_WARN(message); return;
	case GL_DEBUG_SEVERITY_NOTIFICATION: RKT_CORE_TRACE(message); return;
	}

	RKT_CORE_ERROR("Unknown severity level!");
}

void RenderCommand::Init()
{
#ifdef RKT_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
}

void RenderCommand::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void RenderCommand::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
{
	vertexArray->Bind();
	uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, vertexArray->GetIndexBuffer()->GetGlType(), nullptr);
}

void RenderCommand::DrawLine(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
{
	vertexArray->Bind();
	uint32_t count = vertexCount ? vertexCount : vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_LINE_STRIP, count, vertexArray->GetIndexBuffer()->GetGlType(), nullptr);
}

void RenderCommand::SetLineSmooth(bool smooth)
{
	if (smooth)
		glEnable(GL_LINE_SMOOTH);
	else
		glDisable(GL_LINE_SMOOTH);
}

void RenderCommand::SetLineWidth(float width)
{
	glLineWidth(width);
}