#pragma once

#include "glm/glm.hpp"
#include "Material.h"

struct Vertex {
	glm::vec3 Position;
};

struct Triangle {
	Vertex Verticies[3];
	glm::vec3 BoundMin;
	glm::vec3 BoundMax;

	void CalculateBounds() {
		BoundMin = glm::min(Verticies[0].Position, glm::min(Verticies[1].Position, Verticies[2].Position));
		BoundMax = glm::max(Verticies[0].Position, glm::max(Verticies[1].Position, Verticies[2].Position));
	}
};

class Mesh
{
public:
	Mesh(const std::string& path);
	Mesh(Vertex* vertexBuffer, uint16_t* indexBuffer, size_t indexCount, size_t vertexCount);
	~Mesh();

	void SetTranslation(glm::vec3 translation) { m_Translation = translation; RecalculateTransform(); }
	void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; RecalculateTransform(); }
	void SetScale(glm::vec3 scale) { m_Scale = scale; RecalculateTransform(); }

	const glm::vec3& GetTranslation() const { return m_Translation; }
	const glm::vec3& GetRotation() const { return m_Rotation; }
	const glm::vec3& GetScale() const { return m_Scale; }

	const glm::mat4& GetTransform() const { return m_Transform; }

	size_t GetTriangleCount() const { return m_MeshData->IndexCount / 3; };
	void ConstructIthTriangle(size_t i, Triangle& triangle) const;
	
	const glm::vec3& GetBoundMin() const { return m_BoundMin; }
	const glm::vec3& GetBoundMax() const { return m_BoundMax; }
public:
	int MaterialIndex = 0;

private:
	void RecalculateTransform();
	void RecalculateBound();

private:
	struct MeshData {
		Vertex* VertexBuffer;
		uint16_t* IndexBuffer;
		size_t IndexCount;
		size_t VertexCount;

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

	glm::vec3 m_BoundMin;
	glm::vec3 m_BoundMax;

	friend void LoadMesh(const std::string& path, Mesh* mesh); // TODO: change perhaps?
};

