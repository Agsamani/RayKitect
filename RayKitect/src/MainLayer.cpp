#include "rktpch.h"
#include "MainLayer.h"
#include "renderer/RenderCommand.h"
#include "core/Log.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"


MainLayer::MainLayer()
	: Layer("MainLayer"), m_Camera(45.0f, 0.01f, 100.0f)
{
	m_Scene.Spheres.push_back({ {0.0f, 0.0f, 0.0f}, 0.5, {0.35, 0.78, 0.95} });
	m_Scene.Spheres.push_back({ {0.0f, 0.2f, 1.0f}, 0.8, {0.65, 0.88, 0.15} });

}

void MainLayer::OnAttach() 
{
	RenderCommand::SetClearColor({ 0.15, 0.15, 0.15, 1.0 });

	m_Renderer.Init(m_Width, m_Height);
	m_Camera.OnResize(m_Width, m_Height);
}

void MainLayer::OnDetach()
{
	
}

void MainLayer::OnUpdate(float dt)
{
	RenderCommand::Clear();
	if (!m_MainRender) {
		m_Camera.OnUpdate(dt);
		m_Renderer.Render(m_Scene, m_Camera);
	}
}

void MainLayer::OnImGuiUpdate()
{
	ImGui::Begin("Render");
	if (ImGui::Button("RENDER", { ImGui::GetContentRegionAvail().x, 100.0})) {
		m_MainRender = true;
		m_Renderer.OnResize((uint32_t)(m_Width * m_MainRenderMultiplier), (uint32_t)(m_Height * m_MainRenderMultiplier));
		m_Camera.OnResize((uint32_t)(m_Width * m_MainRenderMultiplier), (uint32_t)(m_Height * m_MainRenderMultiplier));

		if (th.joinable()) {
			th.join();
		}
		th = std::thread([&]() {
			this->OnAsyncRender();
			});
		th.detach();
	}
	ImGui::End();

	ImGui::Begin("Scene");
	if (ImGui::Button("Add sphere")) {
		m_Scene.Spheres.push_back({ {0.0f, 0.0f, 0.0f}, 1.0, {1.0, 1.0, 1.0} });
	}
	for (size_t i = 0; i < m_Scene.Spheres.size(); i++) {
		Sphere& sphere = m_Scene.Spheres[i];

		ImGui::PushID(i);

		ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1);
		ImGui::DragFloat("Radius", &sphere.Radius, 0.1);
		ImGui::ColorEdit3("Color", glm::value_ptr(sphere.Color));

		ImGui::Separator();
		ImGui::Separator();

		ImGui::PopID();
	}
		
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Test");
	std::shared_ptr<Texture2D> frameTexture = m_Renderer.GetFrameTexture();

	float multiplier = 1.0f;
	if (!m_MainRender) {
		multiplier = m_MainRenderMultiplier;
	}

	ImGui::Image((ImTextureID)frameTexture->GetRendererID(), { multiplier * frameTexture->GetWidth(), multiplier * frameTexture->GetHeight() },
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	ImGui::PopStyleVar();
}

void MainLayer::OnEvent(Event& e)
{
	
}

void MainLayer::OnAsyncRender()
{
	m_Renderer.Render(m_Scene, m_Camera);
}
