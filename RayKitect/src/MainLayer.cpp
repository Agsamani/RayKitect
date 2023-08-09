#include "rktpch.h"
#include "MainLayer.h"
#include "renderer/RenderCommand.h"
#include "core/Log.h"

#include "glm/glm.hpp"

#include "imgui.h"


MainLayer::MainLayer()
	: Layer("MainLayer"), m_Camera(45.0f, 0.01f, 100.0f)
{

}

void MainLayer::OnAttach() 
{
	RenderCommand::SetClearColor({ 0.15, 0.15, 0.15, 1.0 });

	uint32_t width = 100, height = 100;

	m_Renderer.Init(width, height);
	m_Camera.OnResize(width, height);
}

void MainLayer::OnDetach()
{
	
}

void MainLayer::OnUpdate(float dt)
{
	RenderCommand::Clear();
	m_Camera.OnUpdate(dt);
	m_Renderer.Render(m_Camera);
}

void MainLayer::OnImGuiUpdate()
{
	ImGui::Begin("Render");
	if (ImGui::Button("RENDER", { ImGui::GetContentRegionAvail().x, 100.0})) {
		if (th.joinable()) {
			th.join();
		}
		th = std::thread([&]() {
			this->OnAsyncRender();
			});
		th.detach();
	}
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Test");
	std::shared_ptr<Texture2D> frameTexture = m_Renderer.GetFrameTexture();
	ImGui::Image((ImTextureID)frameTexture->GetRendererID(), { 10.0f * frameTexture->GetWidth(), 10.0f * frameTexture->GetHeight() },
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	ImGui::PopStyleVar();
}

void MainLayer::OnEvent(Event& e)
{
	
}

void MainLayer::OnAsyncRender()
{
	m_Renderer.Render(m_Camera);
}
