#pragma once

#include "core/Base.h"
#include "events/Event.h"

class Window {
public:
	Window(int width = 2048, int height = 1280, std::string title = "RayKitect");
	~Window() = default;
		
	void OnUpdate();

	unsigned int GetWidth() { return m_Data.Width; }
	unsigned int GetHeight() { return m_Data.Height; }

	void SetEventCallback(std::function<void(Event&)> eventCallback) { m_Data.EventCallback = eventCallback; }
	bool IsVsync() { return m_Data.VSync; }
	void SetVSync(bool enabled);

	void* GetNativeWindow() { return m_Window; }
private:
	void Init(int width, int height, std::string title);
	void Shutdown();

	// TODO : clean this
	void* m_Window;

	struct WindowData {
		std::string Title;
		unsigned int Width, Height;
		bool VSync = true;

		std::function<void(Event&)> EventCallback;
	};

	WindowData m_Data;
};