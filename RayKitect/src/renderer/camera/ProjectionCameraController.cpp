#include "rktpch.h"
#include "ProjectionCameraController.h"

#include "core/Base.h"
#include "core/Input.h"
#include "core/Application.h"


// TODO : maybe fov and near and far control
ProjectionCameraController::ProjectionCameraController(float aspectRatio, float fov)
	:m_Camera(fov,aspectRatio, 0.1, 1000.0)
{
}

void ProjectionCameraController::OnUpdate(Deltatime dt)
{
		
	if (Input::IsKeyPressed(Key::W))
	{
		m_Position += m_Camera.GetFrontDirection() * m_CameraMovementSpeed * dt.GetSeconds();
	}
	else if (Input::IsKeyPressed(Key::S))
	{
		m_Position -= m_Camera.GetFrontDirection() * m_CameraMovementSpeed * dt.GetSeconds();
	}

	// TODO : fix cross direction change when angle is > 90.0
	if (Input::IsKeyPressed(Key::A))
	{
		m_Position -= glm::normalize(glm::cross(m_Camera.GetFrontDirection(), glm::vec3(0.0, 1.0, 0.0))) * m_CameraMovementSpeed * dt.GetSeconds();
	}
	else if (Input::IsKeyPressed(Key::D))
	{
		m_Position += glm::normalize(glm::cross(m_Camera.GetFrontDirection(), glm::vec3(0.0, 1.0, 0.0))) * m_CameraMovementSpeed * dt.GetSeconds();
	}

	if (Input::IsKeyPressed(Key::Q))
	{
		m_Position -= glm::vec3(0.0, 1.0, 0.0) * m_CameraMovementSpeed * dt.GetSeconds();
	}
	else if (Input::IsKeyPressed(Key::E))
	{
		m_Position += glm::vec3(0.0, 1.0, 0.0) * m_CameraMovementSpeed * dt.GetSeconds();
	}

	m_Camera.SetPosition(m_Position);

	if (m_MousePressed) {
		glm::vec2 cursorMoveAmount = (Input::GetMousePosition() / glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight())) * 2.0f - glm::vec2(1.0) - m_TempCursorPos;
		m_TempHAngle = cursorMoveAmount.x * m_CameraRotationSpeed;
		m_TempVAngle = cursorMoveAmount.y * m_CameraRotationSpeed;
		m_Camera.SetHorizontalAngle(m_HorizontalAngle + m_TempHAngle);
		m_Camera.SetVerticalAngle(m_VerticalAngle + m_TempVAngle);
	}
}

void ProjectionCameraController::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseButtonPressedEvent>(RKT_BIND_EVENT_FN(ProjectionCameraController::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(RKT_BIND_EVENT_FN(ProjectionCameraController::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<MouseScrolledEvent>(RKT_BIND_EVENT_FN(ProjectionCameraController::OnMouseScrollEvent));
	dispatcher.Dispatch<WindowResizeEvent>(RKT_BIND_EVENT_FN(ProjectionCameraController::OnWindowResizeEvent));

}

void ProjectionCameraController::OnResize(float width, float height)
{
	m_Camera.SetAspectRatio(width / height);
}


bool ProjectionCameraController::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == Mouse::Secondary) {
		m_MousePressed = true;
		m_TempHAngle = m_TempVAngle = 0.0;
		m_TempCursorPos = (Input::GetMousePosition() / glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight())) * 2.0f - glm::vec2(1.0);
		Input::SetCursorVisibility(false);
	}
	return false;
}

bool ProjectionCameraController::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
	if (e.GetMouseButton() == Mouse::Secondary) {
		m_MousePressed = false;
		m_HorizontalAngle += m_TempHAngle;
		m_VerticalAngle += m_TempVAngle;

		m_TempHAngle = m_TempVAngle = 0.0;

		Input::SetCursorPosition((m_TempCursorPos + glm::vec2(1.0)) * 0.5f * glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()));
		Input::SetCursorVisibility(true);
	}
	return false;
}

bool ProjectionCameraController::OnMouseScrollEvent(MouseScrolledEvent& e)
{
	return false;
}

bool ProjectionCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
{
	OnResize((float)e.GetWidth(), (float)e.GetHeight());
	return false;
}