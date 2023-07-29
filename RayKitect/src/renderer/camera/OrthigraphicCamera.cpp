#include "rktpch.h"
#include "OrthigraphicCamera.h"

#include <glm/gtc/matrix_transform.hpp>


OrthographicCamera::OrthographicCamera(float aspectRatio)
	:m_AspectRatio(aspectRatio), m_ProjectionMatrix(glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
{
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::SetAspectRatio(float aspectRatio)
{
	m_AspectRatio = aspectRatio;
	RecalculateProjection();
}

void OrthographicCamera::Recalculate()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_Angle), glm::vec3(0, 0, 1));

	m_ViewMatrix = glm::inverse(transform);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::RecalculateProjection()
{
	m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
}