#include "rktpch.h"
#include "Application.h"

#include "Utils.h"
#include "Base.h"

#include "Renderer/RenderCommand.h"

#include <filesystem>

//Temp 
#include <imgui.h>


Application* Application::m_Instance = nullptr;

Application::Application(const ApplicationSpecification& specification)
	:m_Specification(specification)
{
	if (m_Instance) {
		RKT_CORE_ERROR("Application already exists");
	}
	m_Instance = this;

	// Set working directory here
	if (!m_Specification.WorkingDirectory.empty())
		std::filesystem::current_path(m_Specification.WorkingDirectory);

	m_Window = std::make_unique<Window>(m_Specification.Width, m_Specification.Height, m_Specification.Name);
	m_Window->SetEventCallback(RKT_BIND_EVENT_FN(OnEvent));

	RenderCommand::Init();
	Random::Init();

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);

	RenderCommand::SetViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	RKT_CORE_INFO("Window size: {0}, {1}", m_Window->GetWidth(), m_Window->GetHeight());
}

void Application::run()
{
	while (m_Running)
	{
		float currentTime = Time::GetTime();
		Deltatime dt = currentTime - m_LastFrameTime;
		m_LastFrameTime = currentTime;

		for (Layer* layer : layerStack) {
			layer->OnUpdate(dt);
		}

		m_ImGuiLayer->Begin();
		for (Layer* layer : layerStack) {
			layer->OnImGuiUpdate();
		}
		// Temp 
		ImGui::Begin("Render Time:");
		ImGui::Text("%s %s",std::to_string(dt.GetMilliseconds()).c_str(), "ms");
		ImGui::End();
		//
		m_ImGuiLayer->End();

		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(RKT_BIND_EVENT_FN(OnWindowCloseEvent));
	dispatcher.Dispatch<WindowResizeEvent>(RKT_BIND_EVENT_FN(OnWindowResizeEvent));

	for (auto it = layerStack.rbegin(); it != layerStack.rend(); it++) {
		if (e.Handled) {
			break;
		}
		(*it)->OnEvent(e);
	}
}

void Application::Close()
{
	m_Running = false;
}

void Application::PushLayer(Layer* layer)
{
	layerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
	layerStack.PushOverlay(layer);
}

bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
{
	if (e.GetWidth() == 0 || e.GetHeight() == 0)
	{
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;
		
	RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
	return false;
}

Application::~Application()
{
}
