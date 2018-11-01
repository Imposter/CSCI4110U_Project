#include "Mesh.h"

void Mesh::build()
{
	// TODO: Build
}

Mesh::Mesh(std::string name, std::vector<MeshVertex> vertices, std::vector<unsigned> indices)
	: m_Name(std::move(name)), m_Vertices(std::move(vertices)), m_Indices(std::move(indices)), m_VertexBuffer({}, 0), m_IndexBuffer(0)
{
	build();
}

Mesh::~Mesh()
{
	// TODO: Destroy
}