#pragma once

struct Material {
	glm::vec3 Color{ 0.0f };

	glm::vec3 EmissionColor{ 0.0f };
	float EmissionPower = 0.0f;

	float Smoothnes = 0.0f;

	float Specular = 0.0f;
	glm::vec3 SpecularColor = glm::vec3(1.0f);

	glm::vec3 GetEmission() const { return EmissionColor * EmissionPower; }

};
