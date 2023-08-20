#include "rktpch.h"
#include "MainLayer.h"
#include "renderer/RenderCommand.h"
#include "core/Log.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"

#include "Utils.h"

MainLayer::MainLayer()
	: Layer("MainLayer"), m_Camera(45.0f, 0.01f, 100.0f)
{
	
	Material basicMaterial;
	basicMaterial.Color = glm::vec3(1.0);
	basicMaterial.EmissionColor = glm::vec3(1.0);
	basicMaterial.EmissionPower = 0.0f;
	m_Scene.Materials.push_back(basicMaterial);

	Material emissiveMaterial;
	emissiveMaterial.Color = glm::vec3(1.0);
	emissiveMaterial.EmissionColor = glm::vec3(1.0);
	emissiveMaterial.EmissionPower = 0.0f;
	m_Scene.Materials.push_back(emissiveMaterial);

	Material glass;
	glass.Color = glm::vec3(1.0);
	glass.EmissionColor = glm::vec3(1.0);
	glass.EmissionPower = 0.0f;
	m_Scene.Materials.push_back(glass);

// 	{
// 		Sphere sphere;
// 		sphere.Position = glm::vec3(-1.0f, 0.0f, 0.0f);
// 		sphere.Radius = 1.0f;
// 		sphere.MaterialIndex = 0;
// 		m_Scene.Spheres.push_back(sphere);
// 	}
	{
		Sphere sphere;
		sphere.Position = glm::vec3(0.0f, -101.0f, 0.0f);
		sphere.Radius = 100.0f;
		sphere.MaterialIndex = 0;
		m_Scene.Spheres.push_back(sphere);
	}
// 	{
// 		Sphere sphere;
// 		sphere.Position = glm::vec3(1.0f, 0.0f, 0.0f);
// 		sphere.Radius = 1.0f;
// 		sphere.MaterialIndex = 0;
// 		m_Scene.Spheres.push_back(sphere);
// 	}

	for (int i = 0; i < 1; i++)
	{
		Sphere sphere;
		float r = 3.0f;
		float theta = 2 * 3.1415 * ((float)i / 11.0);

		sphere.Position = glm::vec3(r * glm::cos(theta), -0.8f,r * glm::sin(theta));
		sphere.Radius = 0.2f;
		sphere.MaterialIndex = 0;
		m_Scene.Spheres.push_back(sphere);
	}
//  	{
// 		Sphere sphere;
// 		sphere.Position = glm::vec3(0.0f, 0.0f, 0.0f);
// 		sphere.Radius = 1.0f;
// 		sphere.MaterialIndex = 1;
// 		m_Scene.Spheres.push_back(sphere);
// 	}

	Triangle triangle;
	triangle.Verticies[0] = glm::vec3(0.0f);
	triangle.Verticies[1] = glm::vec3(0.0f, 0.0f, 1.0f);
	triangle.Verticies[2] = glm::vec3(0.0f, 1.0f, 0.0f);
	triangle.MaterialIndex = 1;
	m_Scene.Triangles.push_back(triangle);
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
		if (m_Camera.OnUpdate(dt)) {
			m_Renderer.ResetFrameIndex();
		}
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
	if (ImGui::Button("Reset")) {
		m_Renderer.ResetFrameIndex();
	}
	ImGui::InputInt("Accimulation Count", &m_AccimulationRenderPassCount);
	if (m_MainRender)
	{
		ImGui::Text((std::string("Current render pass: ") + std::to_string(m_CurrentAccimulationRenderPass)).c_str());
	}
	ImGui::End();

	ImGui::Begin("Scene");
	if (ImGui::Button("Add sphere")) {
		m_Scene.Spheres.push_back({ {0.0f, 0.0f, 0.0f}, 1.0, 0});
	}
	for (size_t i = 0; i < m_Scene.Spheres.size(); i++) {
		Sphere& sphere = m_Scene.Spheres[i];

		ImGui::PushID(i);

		ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1);
		ImGui::DragFloat("Radius", &sphere.Radius, 0.1);
		ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, m_Scene.Materials.size() - 1);

		ImGui::Separator();

		ImGui::PopID();
	}
	for (size_t i = 0; i < m_Scene.Triangles.size(); i++) {
		Triangle& triangle = m_Scene.Triangles[i];

		ImGui::PushID(100 * (i + 1));

		ImGui::DragInt("Material", &triangle.MaterialIndex, 1.0f, 0, m_Scene.Materials.size() - 1);

		ImGui::Separator();

		ImGui::PopID();
	}

	if (ImGui::Button("Add material")) {
		m_Scene.Materials.push_back({ {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0} ,0.0 });
	}
	for (size_t i = 0; i < m_Scene.Materials.size(); i++) {
		Material& material = m_Scene.Materials[i];

		ImGui::PushID(i);

		ImGui::ColorEdit3("Color", glm::value_ptr(material.Color));
		ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.EmissionColor));
		ImGui::DragFloat("Emission Power", &material.EmissionPower, 0.1, 0.0, FLT_MAX);
		ImGui::DragFloat("Smoothnes", &material.Smoothnes, 0.02, 0.0f, 1.0f);
		ImGui::DragFloat("Specular", &material.Specular, 0.02, 0.0f, 1.0f);

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
	for (int i = 0; i < m_AccimulationRenderPassCount; i++)
	{
		m_CurrentAccimulationRenderPass = i + 1;
		m_Renderer.Render(m_Scene, m_Camera);
	}
}
