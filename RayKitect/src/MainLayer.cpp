#include "rktpch.h"
#include "MainLayer.h"
#include "renderer/RenderCommand.h"
#include "core/Log.h"

#include "glm/glm.hpp"

#include "imgui.h"

void MainLayer::OnAttach() 
{
	RenderCommand::SetClearColor({ 0.15, 0.15, 0.15, 1.0 });

	m_Renderer.Init(8, 8);
}

void MainLayer::OnDetach()
{
	
}

void MainLayer::OnUpdate(float dt)
{
	RenderCommand::Clear();
}

void MainLayer::OnImGuiUpdate()
{
	ImGui::Begin("Render");
	if (ImGui::Button("RENDER")) {
		m_Renderer.Render();
	}
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Test");
	std::shared_ptr<Texture2D> frameTexture = m_Renderer.GetFrameTexture();
	ImGui::Image((ImTextureID)frameTexture->GetRendererID(), { 100.0f * frameTexture->GetWidth(), 100.0f * frameTexture->GetHeight() },
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	ImGui::PopStyleVar();
}

void MainLayer::OnEvent(Event& e)
{
	
}