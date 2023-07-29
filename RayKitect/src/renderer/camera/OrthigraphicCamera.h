#pragma once

#include <glm/glm.hpp>

class OrthographicCamera {
public:
	OrthographicCamera(float aspectRatio);
	~OrthographicCamera() = default;

	const glm::mat4& GetViewProjection() const { return m_ViewProjectionMatrix; }
	const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
	const glm::mat4& GetView() const { return m_ViewMatrix; }

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(glm::vec3 position) { m_Position = position; Recalculate(); }

	void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; RecalculateProjection(); }
	float GetZoomLevel() const { return m_ZoomLevel; }

	void SetAngle(float angle) { m_Angle = angle; Recalculate(); }
	float GetAngle() const { return m_Angle; }

	void SetAspectRatio(float aspectRatio);
private:
	void Recalculate();
	void RecalculateProjection();

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_Position = glm::vec3(0.0);

	float m_ZoomLevel = 1.0;
	float m_Angle = 0.0;

	float m_AspectRatio;
};

