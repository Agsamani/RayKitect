#include "rktpch.h"
#include "ProjectionCamera.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


ProjectionCamera::ProjectionCamera(float fov, float aspectRatio, float near, float far)
	:m_Fov(glm::radians(fov)),m_AspectRatio(aspectRatio), m_Near(near), m_Far(far)
{
	RecalculateProjection();
	Recalculate();
}

void ProjectionCamera::SetAspectRatio(float aspectRatio)
{
	m_AspectRatio = aspectRatio;
	RecalculateProjection();
}

void ProjectionCamera::Recalculate()
{
	glm::mat4 horizontalRotation = glm::rotate(glm::mat4(1.0), m_HorizontalAngle, glm::vec3(0.0, 1.0, 0.0));
	glm::vec3 tempDir = glm::vec3(horizontalRotation * glm::vec4(0.0, 0.0, -1.0, 1.0));
	glm::mat4 verticalRotation = glm::rotate(glm::mat4(1.0), m_VerticalAngle, (glm::cross(tempDir, glm::vec3(0.0, 1.0, 0.0))));
	m_ViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0), m_Position) * verticalRotation * horizontalRotation);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void ProjectionCamera::RecalculateProjection()
{
	// TODO : proper ortho system
	if (m_Fov > 0.1)
		m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far);
	else
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, m_Near, m_Far);
}

void ProjectionCamera::CalculateFront()
{
	glm::mat4 horizontalRotation = glm::rotate(glm::mat4(1.0), m_HorizontalAngle, glm::vec3(0.0, 1.0, 0.0));
	glm::vec3 tempDir = glm::vec3(horizontalRotation * glm::vec4(0.0, 0.0, -1.0, 1.0));
	glm::mat4 verticalRotation = glm::rotate(glm::mat4(1.0), m_VerticalAngle, (glm::cross(tempDir, glm::vec3(0.0, 1.0, 0.0))));
	m_FrontDirection = glm::vec3(verticalRotation * horizontalRotation * glm::vec4(0.0, 0.0, -1.0, 1.0));
}
