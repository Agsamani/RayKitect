#include "rktpch.h"
#include "Window.h"

#include "GLFW/glfw3.h"

#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "glad/glad.h"

#include "stb_image.h"

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
	RKT_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

Window::Window(int width , int height , std::string title)
{
	Init(width, height, title);
}

void Window::Init(int width, int height, std::string title)
{
	m_Data.Width = width;
	m_Data.Height = height;
	m_Data.Title = title;

	if (s_GLFWWindowCount == 0) {
		if (!glfwInit()) {
			RKT_CORE_ERROR("GLFW error initializing");
		}
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#if defined(RKT_DEBUG)
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	#endif

	m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
	if (!m_Window)
	{
		glfwTerminate();
		RKT_CORE_ERROR("GLFW error creating window");
	}
	++s_GLFWWindowCount;
	glfwMakeContextCurrent((GLFWwindow*)m_Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		RKT_CORE_ERROR("Glad loading failed");
	}
	glfwSetWindowUserPointer((GLFWwindow*)m_Window, &m_Data);
	SetVSync(true);

	// TODO : engine path
	GLFWimage icon;
	icon.pixels = stbi_load("../RayKitect/assets/icons/icon2f.png", &icon.width, &icon.height, nullptr, 0);
	glfwSetWindowIcon((GLFWwindow*)m_Window, 1, &icon);

	glfwSetWindowSizeCallback((GLFWwindow*)m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

	glfwSetWindowCloseCallback((GLFWwindow*)m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

	glfwSetKeyCallback((GLFWwindow*)m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				data.EventCallback(event);
				break;
			}
			}
		});

	glfwSetCharCallback((GLFWwindow*)m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

	glfwSetMouseButtonCallback((GLFWwindow*)m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

	glfwSetScrollCallback((GLFWwindow*)m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

	glfwSetCursorPosCallback((GLFWwindow*)m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

}

void Window::OnUpdate()
{
	glfwSwapBuffers((GLFWwindow*)m_Window);
	glfwPollEvents();
}

void Window::SetVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_Data.VSync = enabled;
}

void Window::Shutdown()
{
	glfwDestroyWindow((GLFWwindow*)m_Window);
	--s_GLFWWindowCount;

	if (s_GLFWWindowCount == 0)
	{
		glfwTerminate();
	}
}