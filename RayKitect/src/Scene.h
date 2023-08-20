#pragma once

#include "glm/glm.hpp"

struct Material {
	glm::vec3 Color;

	glm::vec3 EmissionColor{ 0.0f };
	float EmissionPower = 0.0f;

	float Smoothnes = 0.0f;

	float Specular = 0.0f;
	glm::vec3 SpecularColor = glm::vec3(1.0f);

	glm::vec3 GetEmission() const { return EmissionColor * EmissionPower; }

};

struct Sphere {
	glm::vec3 Position;
	float Radius;

	int MaterialIndex;
};

struct Triangle {
	glm::vec3 Verticies[3];

	int MaterialIndex;
};

struct Scene {
	std::vector<Sphere> Spheres;
	std::vector<Triangle> Triangles;
	std::vector<Material> Materials;
};