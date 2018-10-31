#include "Buffer.h"

Buffer::Buffer(Type type, size_t size, const void *data, bool dynamic)
	: m_ID(0), m_Type(type), m_Size(size), m_Dynamic(dynamic)
{
	// Generate buffer
	glGenBuffers(1, &m_ID);

	// Bind buffer
	glBindBuffer(m_Type, m_ID);

	// Create buffer with no data
	glBufferData(m_Type, m_Size, data, m_Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_ID);
}

const GLuint &Buffer::GetID() const
{
	return m_ID;
}

Buffer::Type Buffer::GetType() const
{
	return m_Type;
}

size_t Buffer::GetSize() const
{
	return m_Size;
}

void Buffer::SetSize(size_t size)
{
	// Allocate a new temporary buffer
	GLuint tempBuffer;
	glGenBuffers(1, &tempBuffer);
	glBindBuffer(m_Type, tempBuffer);

	// Create it to preferred size
	glBufferData(m_Type, m_Size, nullptr, m_Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	// Copy data to it
	glBindBuffer(GL_COPY_READ_BUFFER, m_ID);
	glBindBuffer(GL_COPY_WRITE_BUFFER, tempBuffer);
	glCopyBufferSubData(m_ID, tempBuffer, 0, 0, m_Size);

	// Create our new buffer with no data
	glBindBuffer(m_Type, m_ID);
	glBufferData(m_Type, size, nullptr, m_Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	// Copy data to it
	const auto copySize = size < m_Size ? size : m_Size;
	glBindBuffer(GL_COPY_READ_BUFFER, tempBuffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, m_ID);
	glCopyBufferSubData(tempBuffer, m_ID, 0, 0, copySize);

	// Destroy temporary buffer
	glDeleteBuffers(1, &tempBuffer);

	// Update new size
	m_Size = size;

}

void Buffer::Bind()
{
	glBindBuffer(m_Type, m_ID);
}

const void *Buffer::Map(unsigned int offset, size_t size, Access access)
{
	auto flags = static_cast<int>(access);
	if (flags & kAccess_Write) flags |= GL_MAP_INVALIDATE_RANGE_BIT;
	return glMapBufferRange(m_ID, offset, size, flags);
}

void Buffer::Unmap(unsigned int offset, size_t size)
{
	glFlushMappedBufferRange(m_ID, offset, size);
}