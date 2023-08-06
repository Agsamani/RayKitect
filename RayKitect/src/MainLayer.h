#pragma once
#include "core/Layer.h"
#include "renderer/Texture.h"
#include "Renderer.h"

#include <thread>

class MainLayer : public Layer
{
public:
	MainLayer() : Layer("MainLayer") {};
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
	std::thread th;
};

