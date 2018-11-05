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
}

void VertexArray::shutdown()
{
	// Disable attribute pointers
	if (m_Enabled)
	{
		for (size_t i = 0; i < m_Attributes.size(); i++)
			glDisableVertexAttribArray(i);

		m_Enabled = false;
	}

	// Unbind
	glDeleteVertexArrays(1, &m_ID);
}

VertexArray::VertexArray(std::vector<VertexAttribute> attributes)
	: m_ID(0), m_Attributes(std::move(attributes)), m_Enabled(false), m_Stride(0)
{
	init();
}

VertexArray::~VertexArray()
{
	shutdown();

	m_Attributes.clear();
}

VertexArray::VertexArray(const VertexArray &copy)
	: m_ID(0), m_Attributes(copy.m_Attributes), m_Enabled(copy.m_Enabled), m_Stride(copy.m_Stride)
{
	init();

	if (m_Enabled) Apply();
}

VertexArray &VertexArray::operator=(const VertexArray &copy)
{
	shutdown();

	m_Attributes.clear();

	m_Attributes = copy.m_Attributes;
	m_Stride = copy.m_Stride;

	init();

	if (m_Enabled) Apply();

	return *this;
}

std::vector<VertexAttribute> VertexArray::GetAttributes() const
{
	return m_Attributes;
}

void VertexArray::AddAttribute(const VertexAttribute &attribute)
{
	m_Attributes.push_back(attribute);

	// Update stride
	m_Stride += attribute.GetSize() * attribute.GetCount();
}

void VertexArray::Bind()
{
	// Set active
	glBindVertexArray(m_ID);
}

void VertexArray::Apply()
{
	// Set active
	glBindVertexArray(m_ID);

	unsigned int position = 0;
	for (size_t i = 0; i < m_Attributes.size(); i++)
	{
		const auto &attribute = m_Attributes[i];
		glVertexAttribPointer(i, attribute.GetCount(), attribute.GetType(), attribute.IsNormalized(), m_Stride, reinterpret_cast<void *>(position));
		glEnableVertexAttribArray(i);

		// Update pointer position
		position += attribute.GetSize() * attribute.GetCount();
	}

	m_Enabled = true;
}
