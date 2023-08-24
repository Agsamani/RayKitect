#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <glm/glm.hpp>

#include "json.hpp"

#include "Mesh.h"

using json = nlohmann::json;

static void LoadMesh(const std::string& path, Mesh* mesh) {
	std::ifstream file(path);
	// TODO check path
	json data = json::parse(file);
	auto node = data["nodes"][0];
	//if (node.contains("rotation"))
	int indicesIndex = data["meshes"][(int)node["mesh"]]["primitives"][0]["indices"];
	int positionIndex = data["meshes"][(int)node["mesh"]]["primitives"][0]["attributes"]["POSITION"];

	std::string bufferUri = path.substr(0, path.find_last_of('/') + 1) + (std::string)data["buffers"][0]["uri"];
	uint32_t bufferSize = data["buffers"][0]["byteLength"];
	std::ifstream bufferFile(bufferUri, std::ios::binary);

	uint8_t* buffer = new uint8_t[bufferSize];
	bufferFile.read((char*)buffer, bufferSize);

	auto positionBufferView = data["bufferViews"][(int)data["accessors"][positionIndex]["bufferView"]];
	auto indicesBufferView = data["bufferViews"][(int)data["accessors"][indicesIndex]["bufferView"]];

	mesh->m_MeshData->IndexCount = (int)data["accessors"][indicesIndex]["count"];
	size_t indexBufferSize = sizeof(uint16_t) * (int)data["accessors"][indicesIndex]["count"];
	mesh->m_MeshData->IndexBuffer = new uint16_t[indexBufferSize];
	memcpy(mesh->m_MeshData->IndexBuffer, buffer + (int)indicesBufferView["byteOffset"], indexBufferSize);

	size_t vertexBufferSize = positionBufferView["byteLength"];
	mesh->m_MeshData->VertexBuffer =new Vertex[vertexBufferSize];
	memcpy(mesh->m_MeshData->VertexBuffer, buffer + (int)positionBufferView["byteOffset"], vertexBufferSize);

	bufferFile.close();
	delete[] buffer;
}

