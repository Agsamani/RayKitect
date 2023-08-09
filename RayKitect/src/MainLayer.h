#pragma once
#include "core/Layer.h"
#include "renderer/Texture.h"
#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"

#include <thread>

class MainLayer : public Layer
{
public:
	MainLayer();
	~MainLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnImGuiUpdate() override;
	void OnEvent(Event& e) override;

private:
	void OnAsyncRender();

private:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;

	std::thread th;

	// Temp
	uint32_t m_Width = 160, m_Height = 100;

	float m_MainRenderMultiplier = 10.0;
	bool m_MainRender = false;
};

