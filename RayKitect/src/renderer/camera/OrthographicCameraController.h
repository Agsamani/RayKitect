#pragma once

#include "OrthigraphicCamera.h"
#include "core/Deltatime.h"
#include "events/Event.h"
#include "events/ApplicationEvent.h"
#include "events/MouseEvent.h"
#include <glm/glm.hpp>

class OrthographicCameraController
{
public:
	OrthographicCameraController(float aspectRatio, bool rotationLock = false);
	~OrthographicCameraController() = default;

	void OnUpdate(Deltatime dt);
	void OnEvent(Event& e);

	void OnResize(float width, float height);

	OrthographicCamera& GetCamera() { return m_Camera; }
	const OrthographicCamera& GetCamera() const { return m_Camera; }

private:
	bool OnMouseScrollEvent(MouseScrolledEvent& e);
	bool OnWindowResizeEvent(WindowResizeEvent& e);

	OrthographicCamera m_Camera;

	glm::vec3 m_Position = glm::vec3(0.0);
	float m_Rotation = 0.0;
	float m_ZoomLevel = 1.0;

	float m_MovementSpeed = 1.8, m_RotationSpeed = 50.0, m_ZoomSpeed = 0.25;

	bool m_RotationLock;
};


