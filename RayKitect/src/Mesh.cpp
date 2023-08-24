#include "rktpch.h"
#include "Mesh.h"
#include "MeshLoader.h"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

Mesh::Mesh(const std::string& path)
{
	m_MeshData = std::make_shared<MeshData>();
	LoadMesh(path, this);
}

Mesh::Mesh(Vertex* vertexBuffer, uint16_t* indexBuffer, size_t IndexCount)
{
	m_MeshData = std::make_shared<MeshData>();
	m_MeshData->VertexBuffer = vertexBuffer;
	m_MeshData->IndexBuffer = indexBuffer;
	m_MeshData->IndexCount = IndexCount;
}

Mesh::~Mesh()
{
}

void Mesh::ConstructIthTriangle(size_t i, Triangle& triangle) const
{
	for (int j = 0; j < 3; j++) {
		Vertex vertex = m_MeshData->VertexBuffer[m_MeshData->IndexBuffer[3 * i + j]];
		vertex.Position = glm::vec4(vertex.Position, 1.0f) * m_Transform;
		triangle.Verticies[j] = vertex;
	}
}

void Mesh::RecalculateTransform()
{
	m_Transform = glm::translate(glm::mat4(1.0f), m_Translation)
		* glm::toMat4(glm::quat(m_Rotation))
		* glm::scale(glm::mat4(1.0f), m_Scale);
}
