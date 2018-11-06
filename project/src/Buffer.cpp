#include "Buffer.h"

void Buffer::init(const void *data)
{
	// Generate buffer
	glGenBuffers(1, &m_ID);

	// Bind buffer
	glBindBuffer(m_Target, m_ID);

	// Create buffer with no data
	glBufferData(m_Target, m_Size, data, m_Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

Buffer::Buffer(Target target, size_t size, const void *data, bool dynamic)
	: m_ID(0), m_Target(target), m_Size(size), m_Dynamic(dynamic), m_Mapped(false), m_MappedAccess(kAccess_None)
{
	init(data);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_ID);
}

Buffer::Buffer(const Buffer &copy)
	: m_ID(0), m_Target(copy.m_Target), m_Size(copy.m_Size), m_Dynamic(copy.m_Dynamic), m_Mapped(false), m_MappedAccess(kAccess_None)
{
	// Init
	init();

	// Copy
	Copy(copy);
}

Buffer &Buffer::operator=(const Buffer &copy)
{
	if (m_Mapped)
		THROW_EXCEPTION(BufferMapException, "Cannot set buffer that is currently mapped");

	// Copy other buffer
	Copy(copy);

	return *this;
}

const GLuint &Buffer::GetID() const
{
	return m_ID;
}

Buffer::Target Buffer::GetTarget() const
{
	return m_Target;
}

size_t Buffer::GetSize() const
{
	return m_Size;
}

void Buffer::SetSize(size_t size)
{
	// Create a new buffer
	Buffer tempBuffer(m_Target, size, nullptr, m_Dynamic);

	// Copy current buffer
	tempBuffer.Copy(*this);

	// Copy back to this buffer
	Copy(tempBuffer);
}

void Buffer::Bind(Target target)
{
	if (target == kTarget_None)
		target = m_Target;

	glBindBuffer(target, m_ID);
}

void Buffer::Copy(const Buffer &buffer)
{
	// Bind other buffer as source
	glBindBuffer(GL_COPY_READ_BUFFER, buffer.m_ID);

	// Bind this buffer as write target
	glBindBuffer(GL_COPY_WRITE_BUFFER, m_ID);
	
	// Resize this buffer (if needed)
	if (m_Size != buffer.m_Size)
		glBufferData(GL_COPY_WRITE_BUFFER, buffer.m_Size, nullptr, m_Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	// Copy to this buffer
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, buffer.m_Size);

	// Update size
	m_Size = buffer.m_Size;
}

const void *Buffer::Map(unsigned int offset, size_t size, Access access)
{
	if (m_Mapped)
		THROW_EXCEPTION(BufferMapException, "Buffer already mapped");

	auto flags = static_cast<int>(access);
	if (access & kAccess_Write && !(access & kAccess_Read))
	{
		flags |= GL_MAP_INVALIDATE_RANGE_BIT; // Invalidate the range we are mapping
		flags |= GL_MAP_FLUSH_EXPLICIT_BIT; // Flush on unmap
	}

	glBindBuffer(m_Target, m_ID);
	const auto ptr = glMapBufferRange(m_Target, offset, size, flags);

	m_Mapped = true;
	m_MappedAccess = access;

	return ptr;
}

void Buffer::Unmap(unsigned int offset, size_t size)
{
	if (!m_Mapped)
		THROW_EXCEPTION(BufferMapException, "Buffer not mapped");

	glBindBuffer(m_Target, m_ID);
	if (m_MappedAccess & kAccess_Write && !(m_MappedAccess & kAccess_Read)) 
		glFlushMappedBufferRange(m_Target, offset, size);
	glUnmapBuffer(m_Target);

	m_Mapped = false;
	m_MappedAccess = kAccess_None;
}