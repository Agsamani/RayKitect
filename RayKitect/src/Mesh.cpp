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
	RecalculateBound();
}

Mesh::Mesh(Vertex* vertexBuffer, uint16_t* indexBuffer, size_t indexCount, size_t vertexCount) // TODO : change to accept mesh data structure
{
	m_MeshData = std::make_shared<MeshData>();
	m_MeshData->VertexBuffer = vertexBuffer;
	m_MeshData->IndexBuffer = indexBuffer;
	m_MeshData->IndexCount = indexCount;
	m_MeshData->VertexCount = vertexCount;
	RecalculateBound();
}

Mesh::~Mesh()
{
}

void Mesh::ConstructIthTriangle(size_t i, Triangle& triangle) const
{
	for (int j = 0; j < 3; j++) {
		Vertex vertex = m_MeshData->VertexBuffer[m_MeshData->IndexBuffer[3 * i + j]];
		vertex.Position = glm::vec3(m_Transform * glm::vec4(vertex.Position, 1.0f));
		triangle.Verticies[j] = vertex;
	}
	triangle.CalculateBounds();
}

void Mesh::RecalculateTransform()
{
	m_Transform = glm::translate(glm::mat4(1.0f), m_Translation)
		* glm::toMat4(glm::quat(m_Rotation))
		* glm::scale(glm::mat4(1.0f), m_Scale);

	RecalculateBound();
}

void Mesh::RecalculateBound()
{
	glm::vec3 minTemp = glm::vec3(FLT_MAX);
	glm::vec3 maxTemp = glm::vec3(FLT_MIN);

	for (int i = 0; i < m_MeshData->VertexCount; i++) {
		const glm::vec3& v = glm::vec3(m_Transform * glm::vec4(m_MeshData->VertexBuffer[i].Position, 1.0f));

		minTemp = glm::min(minTemp, v);
		maxTemp = glm::max(maxTemp, v);
	}

	m_BoundMin = minTemp;
	m_BoundMax = maxTemp;
}
