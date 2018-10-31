#pragma once

#include "Buffer.h"
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>

enum VertexAttributeType
{
	kVertexAttributeType_Byte = GL_BYTE,
	kVertexAttributeType_UnsignedByte = GL_UNSIGNED_BYTE,
	kVertexAttributeType_Short = GL_SHORT,
	kVertexAttributeType_UnsignedShort = GL_UNSIGNED_SHORT,
	kVertexAttributeType_Int = GL_INT,
	kVertexAttributeType_UnsignedInt = GL_UNSIGNED_INT,
	kVertexAttributeType_Float = GL_FLOAT,
	kVertexAttributeType_Double = GL_DOUBLE
};

class VertexAttribute
{
	std::string m_Name;
	VertexAttributeType m_Type;
	unsigned int m_Count;
	bool m_Normalized;
	size_t m_Size;

public:
	VertexAttribute(std::string name, VertexAttributeType type, unsigned int count, bool normalized, size_t size);
	const std::string &GetName() const;
	VertexAttributeType GetType() const;
	unsigned int GetCount() const;
	bool IsNormalized() const;
	size_t GetSize() const;
};

class VertexArray
{
	GLuint m_ID;
	std::vector<VertexAttribute> m_Attributes;
	bool m_Enabled;
	unsigned int m_Stride;

public:
	VertexArray(std::vector<VertexAttribute> attributes = {});
	~VertexArray();

	std::vector<VertexAttribute> GetAttributes() const;
	void AddAttribute(const VertexAttribute &attribute);

	void Bind();

	// NOTE: An array buffer (VBO) for this array should already be bound before calling this function, as well as this array being binded
	void Apply();
};

// In OpenGL 3.0, every Vertex Buffer needs its own VertexArray
template<typename TVertex>
class VertexBuffer : public Buffer
{
	VertexArray m_Array;
	unsigned int m_Count;

public:
	VertexBuffer(std::vector<VertexAttribute> attributes, unsigned int count)
		: Buffer(kType_ArrayBuffer, sizeof(TVertex) * count), m_Array(attributes), m_Count(count)
	{
	}

	VertexBuffer(std::vector<VertexAttribute> attributes, const void *data, unsigned int count)
		: Buffer(kType_ArrayBuffer, sizeof(TVertex) * count, data), m_Array(attributes), m_Count(count)
	{
	}

	~VertexBuffer() = default;

	VertexBuffer(const VertexBuffer &) = delete;
	VertexBuffer &operator=(const VertexBuffer &) = delete;

	VertexBuffer(const VertexBuffer &&) = delete;
	VertexBuffer &operator=(const VertexBuffer &&) = delete;

	const TVertex *Map(unsigned int index, unsigned int count, Access access = kAccess_ReadWrite)
	{
		return Buffer::Map<TVertex>(index, count, access);
	}

	void Unmap(unsigned int index, unsigned int count)
	{
		Buffer::Unmap<TVertex>(index, count);
	}

	unsigned int GetCount() const
	{
		return m_Count;
	}
};

class IndexBuffer : public Buffer
{
	unsigned int m_Count;

public:
	IndexBuffer(unsigned int count)
		: Buffer(kType_ElementArrayBuffer, sizeof(glm::vec3) * count), m_Count(count)
	{
	}

	IndexBuffer(const void *data, unsigned int count)
		: Buffer(kType_ElementArrayBuffer, sizeof(glm::vec3) * count, data), m_Count(count)
	{
	}

	~IndexBuffer() = default;

	IndexBuffer(const IndexBuffer &) = delete;
	IndexBuffer &operator=(const IndexBuffer &) = delete;

	IndexBuffer(const IndexBuffer &&) = delete;
	IndexBuffer &operator=(const IndexBuffer &&) = delete;

	const glm::vec3 *Map(unsigned int index, unsigned int count, Access access = kAccess_ReadWrite)
	{
		return Buffer::Map<glm::vec3>(index, count, access);
	}

	void Unmap(unsigned int index, unsigned int count)
	{
		Buffer::Unmap<glm::vec3>(index, count);
	}

	unsigned int GetCount() const
	{
		return m_Count;
	}
};