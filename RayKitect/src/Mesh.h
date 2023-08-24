#pragma once

#include "glm/glm.hpp"
#include "Material.h"

struct Vertex {
	glm::vec3 Position;
};

struct Triangle {
	Vertex Verticies[3];
};

class Mesh
{
public:
	Mesh(const std::string& path);
	Mesh(Vertex* VertexBuffer, uint16_t* IndexBuffer, size_t IndexCount);
	~Mesh();

	void SetTranslation(glm::vec3 translation) { m_Translation = translation; RecalculateTransform(); }
	void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; RecalculateTransform(); }
	void SetScale(glm::vec3 scale) { m_Scale = scale; RecalculateTransform(); }

	const glm::mat4& GetTransform() const { return m_Transform; }

	size_t GetTriangleCount() const { return m_MeshData->IndexCount / 3; };
	void ConstructIthTriangle(size_t i, Triangle& triangle) const;
	
public:
	int MaterialIndex = 0;

private:
	void RecalculateTransform();

private:
	struct MeshData {
		Vertex* VertexBuffer;
		uint16_t* IndexBuffer;
		size_t IndexCount;

		~MeshData() {
			delete VertexBuffer;
			delete IndexBuffer;
		}
	};

	std::shared_ptr<MeshData> m_MeshData;

	glm::vec3 m_Translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

	glm::mat4 m_Transform = glm::mat4(1.0);

	friend void LoadMesh(const std::string& path, Mesh* mesh); // TODO: change perhaps?
};

