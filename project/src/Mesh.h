#pragma once

#include "Vertex.h"
#include <glm/glm.hpp>

struct MeshVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	MeshVertex(glm::vec3 p, glm::vec3 n, glm::vec2 t)
		: Position(p), Normal(n), TexCoords(t)
	{
	}
};

class Mesh
{
	std::string m_Name;
	std::vector<MeshVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	VertexArray m_VertexArray; // VAO
	VertexBuffer<MeshVertex> m_VertexBuffer; // VBO
	IndexBuffer m_IndexBuffer; // EBO

	void build();

public:
	// ...
	Mesh(std::string name, std::vector<MeshVertex> vertices, std::vector<unsigned int> indices);
	~Mesh();
};