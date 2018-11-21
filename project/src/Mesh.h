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
};

class MeshVertexFormat : public VertexFormat<MeshVertex>
{
public:
	MeshVertexFormat()
		: VertexFormat<MeshVertex>({
			{ "Position", kVertexAttributeType_Float, 3, false, sizeof(float) },
			{ "Normal", kVertexAttributeType_Float, 3, false, sizeof(float) },
			{ "TexCoords", kVertexAttributeType_Float, 2, false, sizeof(float) }
			})
	{
	}
};

// TODO: Add ability to change material for meshes
template<typename TVertex, typename TVertexFormat>
class IMesh : public Node
{
	std::vector<TVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	Material *m_Material;

	TVertexFormat m_VertexFormat;
	VertexArray *m_VertexArray; // VAO
	VertexBuffer<TVertex> m_VertexBuffer; // VBO
	IndexBuffer m_IndexBuffer; // EBO

public:
	IMesh(std::string name, std::vector<TVertex> vertices, std::vector<unsigned int> indices, Material *material = nullptr)
		: Node(name), m_Vertices(std::move(vertices)), m_Indices(std::move(indices)),
		m_Material(material), m_VertexFormat(), m_VertexArray(m_VertexFormat.GetArray()),
		m_VertexBuffer(m_VertexArray, m_Vertices.data(), m_Vertices.size()),
		m_IndexBuffer(m_Indices.data(), m_Indices.size())
	{
	}

	~IMesh() = default;

	// No copying/moving
	IMesh(const IMesh &) = delete;
	IMesh &operator=(const IMesh &) = delete;

	IMesh(const IMesh &&) = delete;
	IMesh &operator=(const IMesh &&) = delete;

	Material *GetMaterial() const
	{
		return m_Material;
	}

	// TODO: Implement?
	// Use with caution! Memory here is unmanaged
	// we need a way to store the material in the 
	// model so it is destroyed automatically on 
	// shutdown
	/*void SetMaterial(Material *material)
	{
		m_Material = material;
	}*/

	void Compile() override
	{
		// Call compile for all children
		Node::Compile();
	}

	void Render(RenderContext *context) override
	{
		// Apply material
		if (m_Material)	m_Material->Apply();

		// Bind vertex array
		m_VertexArray->Bind();
		m_VertexBuffer.Bind();
		m_IndexBuffer.Bind();

		// Render
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr); // TODO: Move these to graphics manager, along with buffers, etc.
		// TODO: create buffers through buffermanager

		// Call render for all children
		Node::Render(context);
	}
};

class Mesh : public IMesh<MeshVertex, MeshVertexFormat>
{
public:
	Mesh(std::string name, std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, Material *material = nullptr);
	~Mesh() = default;

	// No copying/moving
	Mesh(const Mesh &) = delete;
	Mesh &operator=(const Mesh &) = delete;

	Mesh(const Mesh &&) = delete;
	Mesh &operator=(const Mesh &&) = delete;
};