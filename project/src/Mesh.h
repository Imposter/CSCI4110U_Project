#pragma once

#include "Vertex.h"
#include "Material.h"
#include "Node.h"
#include <glm/glm.hpp>

struct MeshVertex
{
	glm::vec3 Position; // Float 3
	glm::vec3 Normal; // Float 3
	glm::vec2 TexCoords; // Float 2

	MeshVertex();
	MeshVertex(glm::vec3 p, glm::vec3 n, glm::vec2 t);

	static std::vector<VertexAttribute> GetAttributes();
};

class Mesh : public Node
{
	std::vector<MeshVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	Material *m_Material;

	VertexArray m_VertexArray; // VAO
	VertexBuffer<MeshVertex> m_VertexBuffer; // VBO
	IndexBuffer m_IndexBuffer; // EBO

public:
	Mesh(std::string name, std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, Material *material = nullptr);
	~Mesh() = default;

	// No copying/moving
	Mesh(const Mesh &) = delete;
	Mesh &operator=(const Mesh &) = delete;

	Mesh(const Mesh &&) = delete;
	Mesh &operator=(const Mesh &&) = delete;

	void Compile() override;
	void Render(RenderContext *context) override;
};