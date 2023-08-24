#pragma once

#include "glm/glm.hpp"

#include "Mesh.h"
#include "Material.h"



struct Sphere {
	glm::vec3 Position;
	float Radius;

	int MaterialIndex;
};

struct Scene {
	std::vector<Sphere> Spheres;
	std::vector<Mesh> Meshes;
	std::vector<Material> Materials;
};