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

void Buffer::Bind()
{
	glBindBuffer(m_Type, m_ID);
}

const void *Buffer::Map(unsigned offset, size_t size, int access)
{
	auto flags = access;
	if (flags & kAccess_Write) flags |= GL_MAP_INVALIDATE_RANGE_BIT;
	return glMapBufferRange(m_ID, offset, size, flags);
}

void Buffer::Unmap(unsigned offset, size_t size)
{
	glFlushMappedBufferRange(m_ID, offset, size);
}