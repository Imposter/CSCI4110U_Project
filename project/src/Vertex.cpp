#include "Vertex.h"

VertexAttribute::VertexAttribute(std::string name, VertexAttributeType type, unsigned int count, bool normalized, size_t size)
	: m_Name(name), m_Type(type), m_Count(count), m_Normalized(normalized), m_Size(size)
{
}

const std::string &VertexAttribute::GetName() const
{
	return m_Name;
}

VertexAttributeType VertexAttribute::GetType() const
{
	return m_Type;
}

unsigned int VertexAttribute::GetCount() const
{
	return m_Count;
}

bool VertexAttribute::IsNormalized() const
{
	return m_Normalized;
}

size_t VertexAttribute::GetSize() const
{
	return m_Size;
}