#include "Vertex.h"

VertexAttribute::VertexAttribute(std::string name, VertexAttributeType type, unsigned int count, bool normalized, size_t size)
	: m_Name(std::move(name)), m_Type(type), m_Count(count), m_Normalized(normalized), m_Size(size)
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

void VertexArray::init()
{
	// Generate vertex array
	glGenVertexArrays(1, &m_ID);

	// Bind vertex array
	glBindVertexArray(m_ID);

	unsigned int position = 0;
	for (size_t i = 0; i < m_Attributes.size(); i++)
	{
		const auto &attribute = m_Attributes[i];
		glEnableVertexAttribArray(i);
		glVertexAttribFormat(i, attribute.GetCount(), attribute.GetType(), attribute.IsNormalized(), position);
		glVertexAttribBinding(i, 0); // Use binding for VBO 0

		// Update pointer position
		position += attribute.GetSize() * attribute.GetCount();
	}
}

void VertexArray::shutdown()
{
	// Disable attribute pointers
	for (size_t i = 0; i < m_Attributes.size(); i++)
		glDisableVertexAttribArray(i);

	// Unbind
	glDeleteVertexArrays(1, &m_ID);
}

VertexArray::VertexArray(std::vector<VertexAttribute> attributes)
	: m_ID(0), m_Attributes(std::move(attributes)), m_Stride(0)
{
	init();

	// Update stride
	for (auto &attribute : m_Attributes)
		m_Stride += attribute.GetSize() * attribute.GetCount();
}

VertexArray::~VertexArray()
{
	shutdown();

	m_Attributes.clear();
}

VertexArray::VertexArray(const VertexArray &copy)
	: m_ID(0), m_Attributes(copy.m_Attributes), m_Stride(copy.m_Stride)
{
	init();
}

VertexArray &VertexArray::operator=(const VertexArray &copy)
{
	shutdown();

	m_Attributes.clear();

	m_Attributes = copy.m_Attributes;
	m_Stride = copy.m_Stride;

	init();

	return *this;
}

std::vector<VertexAttribute> VertexArray::GetAttributes() const
{
	return m_Attributes;
}

unsigned int VertexArray::GetStride() const
{
	return m_Stride;
}

void VertexArray::Bind()
{
	// Set active
	glBindVertexArray(m_ID);
}
