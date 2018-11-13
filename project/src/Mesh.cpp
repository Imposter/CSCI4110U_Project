#include "Mesh.h"
#include <utility>

MeshVertex::MeshVertex()
	: Position(0.0f), Normal(0.0f), TexCoords(0.0f)
{
}

MeshVertex::MeshVertex(glm::vec3 p, glm::vec3 n, glm::vec2 t)
	: Position(p), Normal(n), TexCoords(t)
{
}

Mesh::Mesh(std::string name, std::vector<MeshVertex> vertices, std::vector<unsigned> indices, Material *material)
	: IMesh(std::move(name), std::move(vertices), std::move(indices), material)
{
}