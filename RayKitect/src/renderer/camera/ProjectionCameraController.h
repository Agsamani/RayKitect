#pragma once

#include "ProjectionCamera.h"
#include "core/Deltatime.h"
#include "events/Event.h"
#include "events/ApplicationEvent.h"
#include "events/MouseEvent.h"
#include <glm/glm.hpp>

class ProjectionCameraController
{
public:
	ProjectionCameraController(float aspectRatio, float fov);
	~ProjectionCameraController() = default;

	void OnUpdate(Deltatime dt);
	void OnEvent(Event& e);

	void OnResize(float width, float height);

	ProjectionCamera& GetCamera() { return m_Camera; }
	const ProjectionCamera& GetCamera() const { return m_Camera; }

private:
	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
	bool OnMouseScrollEvent(MouseScrolledEvent& e);
	bool OnWindowResizeEvent(WindowResizeEvent& e);

	ProjectionCamera m_Camera;

	glm::vec3 m_Position = glm::vec3(0.0);
	float m_HorizontalAngle = 0.0, m_VerticalAngle = 0.0;

	glm::vec2 m_TempCursorPos;
	float m_TempHAngle = 0.0, m_TempVAngle = 0.0;
	bool m_MousePressed = false;

	float m_CameraMovementSpeed = 4.0, m_CameraRotationSpeed = 50.0;
};