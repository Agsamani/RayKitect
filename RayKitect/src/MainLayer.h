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
};

