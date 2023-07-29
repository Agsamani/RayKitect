#include <rktpch.h>
#include "Input.h"

#include "GLFW/glfw3.h"
#include "Application.h"

bool Input::IsKeyPressed(KeyCode key)
{
	auto* glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetKey(glfwWindow, static_cast<int32_t>(key));
	return state == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(MouseCode button)
{
	auto* glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetMouseButton(glfwWindow, static_cast<int32_t>(button));
	return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
	auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}

float Input::GetMouseX()
{
	return GetMousePosition().x;
}

float Input::GetMouseY()
{
	return GetMousePosition().y;
}

void Input::SetCursorPosition(const glm::vec2& pos)
{
	auto* glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	glfwSetCursorPos(glfwWindow ,pos.x, pos.y);
}

void Input::SetCursorVisibility(bool show)
{
	auto* glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	if (show) {
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}
