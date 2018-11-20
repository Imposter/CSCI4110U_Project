#pragma once

#include "Buffer.h"
#include <string>
#include <vector>
#include <GL/glew.h>

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
	unsigned int m_Stride;

	void init();
	void shutdown();

public:
	VertexArray(std::vector<VertexAttribute> attributes = {});
	~VertexArray();

	VertexArray(const VertexArray &copy);
	VertexArray &operator=(const VertexArray &copy);

	// No moving
	VertexArray(const VertexArray &&) = delete;
	VertexArray &operator=(const VertexArray &&) = delete;

	std::vector<VertexAttribute> GetAttributes() const;
	unsigned int GetStride() const;

	void Bind();
};

// In OpenGL 3.0, every Vertex Buffer needs its own VertexArray?
template<typename TVertex>
class VertexBuffer : public Buffer
{
	VertexArray *m_Array;
	unsigned int m_Count;

public:
	VertexBuffer(VertexArray *vertexArray, unsigned int count)
		: Buffer(kTarget_ArrayBuffer, kUsage_DynamicDraw, sizeof(TVertex) * count), m_Array(vertexArray), m_Count(count)
	{
	}

	VertexBuffer(VertexArray *vertexArray, const void *data, unsigned int count)
		: Buffer(kTarget_ArrayBuffer, kUsage_DynamicDraw, sizeof(TVertex) * count, data), m_Array(vertexArray), m_Count(count)
	{
	}

	~VertexBuffer() = default;

	VertexBuffer(const VertexBuffer &copy)
		: Buffer(copy), m_Array(copy.m_Array), m_Count(copy.m_Count)
	{
	}

	VertexBuffer &operator=(const VertexBuffer &copy)
	{
		Buffer::operator=(copy);

		m_Array = copy.m_Array;
		m_Count = copy.m_Count;

		return *this;
	}

	// No moving
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

	void Bind()
	{
		glBindVertexBuffer(0, m_ID, 0, m_Array->GetStride());
	}
};

class IndexBuffer : public Buffer
{
	unsigned int m_Count;

public:
	IndexBuffer(unsigned int count)
		: Buffer(kTarget_ElementArrayBuffer, kUsage_DynamicDraw, sizeof(unsigned int) * count), m_Count(count)
	{
	}

	IndexBuffer(const void *data, unsigned int count)
		: Buffer(kTarget_ElementArrayBuffer, kUsage_DynamicDraw, sizeof(unsigned int) * count, data), m_Count(count)
	{
	}

	~IndexBuffer() = default;

	IndexBuffer(const IndexBuffer &copy)
		: Buffer(copy), m_Count(copy.m_Count)
	{
	}

	IndexBuffer &operator=(const IndexBuffer &copy)
	{
		Buffer::operator=(copy);

		m_Count = copy.m_Count;

		return *this;
	}

	// No moving
	IndexBuffer(const IndexBuffer &&) = delete;
	IndexBuffer &operator=(const IndexBuffer &&) = delete;

	const unsigned int *Map(unsigned int index, unsigned int count, Access access = kAccess_ReadWrite)
	{
		return Buffer::Map<unsigned int>(index, count, access);
	}

	void Unmap(unsigned int index, unsigned int count)
	{
		Buffer::Unmap<unsigned int>(index, count);
	}

	unsigned int GetCount() const
	{
		return m_Count;
	}
};

template<typename TVertex>
class VertexFormat
{
	std::vector<VertexAttribute> m_Attributes;

public:
	VertexFormat(std::vector<VertexAttribute> attributes)
		: m_Attributes(attributes)
	{
	}

	VertexArray *GetArray() const
	{
		static VertexArray instance(m_Attributes);
		return &instance;
	}
};