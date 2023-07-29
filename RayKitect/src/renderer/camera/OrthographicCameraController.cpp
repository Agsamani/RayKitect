#include "rktpch.h"
#include "OrthographicCameraController.h"

#include "core/Base.h"
#include "core/Input.h"
#include "core/Application.h"


OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationLock)
	:m_Camera(aspectRatio), m_RotationLock(rotationLock)
{

}

void OrthographicCameraController::OnUpdate(Deltatime dt)
{
	if (Input::IsKeyPressed(Key::A))
	{
		m_Position.x -= cos(glm::radians(m_Rotation)) * m_MovementSpeed * dt;
		m_Position.y -= sin(glm::radians(m_Rotation)) * m_MovementSpeed * dt;
	}
	else if (Input::IsKeyPressed(Key::D))
	{
		m_Position.x += cos(glm::radians(m_Rotation)) * m_MovementSpeed * dt;
		m_Position.y += sin(glm::radians(m_Rotation)) * m_MovementSpeed * dt;
	}

	if (Input::IsKeyPressed(Key::W))
	{
		m_Position.x += -sin(glm::radians(m_Rotation)) * m_MovementSpeed * dt;
		m_Position.y += cos(glm::radians(m_Rotation)) * m_MovementSpeed * dt;
	}
	else if (Input::IsKeyPressed(Key::S))
	{
		m_Position.x -= -sin(glm::radians(m_Rotation)) * m_MovementSpeed * dt;
		m_Position.y -= cos(glm::radians(m_Rotation)) * m_MovementSpeed * dt;
	}

	if (!m_RotationLock)
	{
		if (Input::IsKeyPressed(Key::Q))
			m_Rotation += m_RotationSpeed * dt;
		if (Input::IsKeyPressed(Key::E))
			m_Rotation -= m_RotationSpeed * dt;

		if (m_Rotation > 180.0f)
			m_Rotation -= 360.0f;
		else if (m_Rotation <= -180.0f)
			m_Rotation += 360.0f;

		m_Camera.SetAngle(m_Rotation);
	}

	m_Camera.SetPosition(m_Position);

	m_MovementSpeed = m_ZoomLevel;
}

void OrthographicCameraController::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(RKT_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrollEvent));
	dispatcher.Dispatch<WindowResizeEvent>(RKT_BIND_EVENT_FN(OrthographicCameraController::OnWindowResizeEvent));
}

void OrthographicCameraController::OnResize(float width, float height)
{
	m_Camera.SetAspectRatio(width / height);
}


bool OrthographicCameraController::OnMouseScrollEvent(MouseScrolledEvent& e)
{
	m_ZoomLevel -= e.GetYOffset() * m_ZoomSpeed;
	m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
	m_Camera.SetZoomLevel(m_ZoomLevel);
	return false;
}

bool OrthographicCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
{
	OnResize((float)e.GetWidth(), (float)e.GetHeight());
	return false;
}
