#include "rktpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "core/Input.h"

Camera::Camera(float verticalFOV, float nearClip, float farClip)
	: m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
{
	m_ForwardDirection = glm::vec3(0, 0, -1);
	m_Position = glm::vec3(0, 0, 6);
}

void Camera::OnUpdate(float dt)
{
	glm::vec2 cursorPos = Input::GetMousePosition();
	glm::vec2 delta = (cursorPos - m_LastMousePosition) * 0.01f;
	m_LastMousePosition = cursorPos;

	if (!Input::IsMouseButtonPressed(Mouse::Secondary)) {
		Input::SetCursorVisibility(true);
		return;
	}

	Input::SetCursorVisibility(false);

	glm::vec3 upDirection = { 0.0f, 1.0f, 0.0f };
	glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

	if (Input::IsKeyPressed(Key::W))
	{
		m_Position += m_ForwardDirection * m_CameraMovementSpeed * dt;
	}
	else if (Input::IsKeyPressed(Key::S))
	{
		m_Position -= m_ForwardDirection * m_CameraMovementSpeed * dt;
	}

	if (Input::IsKeyPressed(Key::A))
	{
		m_Position -= rightDirection * m_CameraMovementSpeed * dt;
	}
	else if (Input::IsKeyPressed(Key::D))
	{
		m_Position += rightDirection * m_CameraMovementSpeed * dt;
	}

	if (Input::IsKeyPressed(Key::Q))
	{
		m_Position -= upDirection * m_CameraMovementSpeed * dt;
	}
	else if (Input::IsKeyPressed(Key::E))
	{
		m_Position += upDirection * m_CameraMovementSpeed * dt;
	}

	if (delta.x != 0.0f || delta.y != 0.0f)
	{
		float pitchDelta = delta.y * m_CameraRotationSpeed;
		float yawDelta = delta.x * m_CameraRotationSpeed;

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(pitchDelta, rightDirection),
			glm::angleAxis(yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
		m_ForwardDirection = glm::rotate(q, m_ForwardDirection);
	}

	RecalculateView();
}

void Camera::OnResize(uint32_t width, uint32_t height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight)
		return;

	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateProjection();
}

void Camera::RecalculateProjection()
{
	m_Projection = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_NearClip, m_FarClip);
	m_InverseProjection = glm::inverse(m_Projection);
}

void Camera::RecalculateView()
{
	m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0, 1, 0));
	m_InverseView = glm::inverse(m_View);
}

glm::vec3 Camera::GetRayDirection(const uint32_t& x, const uint32_t& y) const
{
	glm::vec2 coord = { (float)x / m_ViewportWidth, (float)y / m_ViewportHeight };
	coord = coord * 2.0f - 1.0f;

	glm::vec4 target = m_InverseProjection * glm::vec4(coord.x, coord.y, 1, 1);
	glm::vec3 rayDirection = glm::vec3(m_InverseView * glm::vec4(glm::vec3(target) / target.w, 0));
	return rayDirection;
}
