#pragma once

#include <glm/glm.hpp>


class ProjectionCamera
{
public:
	ProjectionCamera(float fov, float aspectRatio, float near, float far);
	~ProjectionCamera() = default;
			

	const glm::mat4& GetViewProjection() { return m_ViewProjectionMatrix; }
	const glm::mat4& GetProjection() { return m_ProjectionMatrix; }
	const glm::mat4& GetView() { return m_ViewMatrix; }

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(glm::vec3 position) { m_Position = position; Recalculate();}

	float GetHorizontalAngle() const { return m_HorizontalAngle; }
	void SetHorizontalAngle(float angle) { m_HorizontalAngle = glm::radians(angle); Recalculate(); }
	float GetVerticalAngle() const { return m_VerticalAngle; }
	void SetVerticalAngle(float angle) { m_VerticalAngle = glm::radians(angle); Recalculate(); }

	const glm::vec3& GetFrontDirection() { CalculateFront(); return m_FrontDirection; }

	void SetAspectRatio(float aspectRatio);
private:
	void Recalculate();
	void RecalculateProjection();
	void CalculateFront();

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_FrontDirection = glm::vec3(0.0, 0.0, -1.0);

	glm::vec3 m_Position = glm::vec3(0.0);
	float m_HorizontalAngle = 0.0, m_VerticalAngle = 0.0;

	float m_Fov;
	float m_Near, m_Far;

	float m_AspectRatio;
};