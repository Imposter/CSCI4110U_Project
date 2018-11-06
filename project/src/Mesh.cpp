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

std::vector<VertexAttribute> MeshVertex::GetAttributes()
{
	return {
		{ "Position", kVertexAttributeType_Float, 3, false, sizeof(float) },
		{ "Normal", kVertexAttributeType_Float, 3, false, sizeof(float) },
		{ "TexCoords", kVertexAttributeType_Float, 2, false, sizeof(float) }
	};
}

Mesh::Mesh(std::string name, std::vector<MeshVertex> vertices, std::vector<unsigned> indices, Material *material)
	: Node(name), m_Vertices(std::move(vertices)), m_Indices(std::move(indices)), 
	m_Material(material), m_VertexArray(MeshVertex::GetAttributes()), 
	m_VertexBuffer(&m_VertexArray, m_Vertices.data(), m_Vertices.size()),
	m_IndexBuffer(m_Indices.data(), m_Indices.size())
{
}

void Mesh::Compile()
{
	// Call compile for all children
	Node::Compile();

	// Bind vertices, and indices, then apply vertex array
	m_VertexArray.Bind();
	m_VertexBuffer.Bind();
	m_IndexBuffer.Bind();
	m_VertexArray.Apply();
}

void Mesh::Render(RenderContext *context)
{
	// Apply material
	if (m_Material)	m_Material->Apply();

	// Bind vertex array
	m_VertexArray.Bind();

	// Render
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr);

	// Call render for all children
	Node::Render(context);
}