#pragma once

#include <cstdint>
#include <GL/glew.h>

// TODO/NOTE: We could also use this in textures, which would allow us to monitor video memory too since we'd be using it for everything
class Buffer // TODO: Resizing
{
public:
	enum Type
	{
		kType_ArrayBuffer = GL_ARRAY_BUFFER,
		kType_ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
		kType_QueryBuffer = GL_QUERY_BUFFER,
		kType_TextureBuffer = GL_TEXTURE_BUFFER,
	};

	enum Access
	{
		kAccess_Read = GL_MAP_READ_BIT,
		kAccess_Write = GL_MAP_WRITE_BIT,
		kAccess_ReadWrite = kAccess_Read | kAccess_Write
	};

private:
	GLuint m_ID;
	Type m_Type;
	size_t m_Size;
	bool m_Dynamic;

public:
	Buffer(Type type, size_t size, const void *data = nullptr, bool dynamic = false);
	~Buffer();

	// No copying/moving -- for now
	Buffer(const Buffer &) = delete;
	Buffer &operator=(const Buffer &) = delete;

	Buffer(const Buffer &&) = delete;
	Buffer &operator=(const Buffer &&) = delete;

	const GLuint &GetID() const;
	Type GetType() const;
	size_t GetSize() const;
	void SetSize(size_t size);

	void Bind();

	const void *Map(unsigned int offset, size_t size, Access access = kAccess_ReadWrite);
	void Unmap(unsigned int offset, size_t size);

	template<typename TObject>
	const TObject *Map(unsigned int index, unsigned int count, Access access = kAccess_ReadWrite)
	{
		return Map(index * sizeof(TObject), count * sizeof(TObject), access);
	}

	template<typename TObject>
	void Unmap(unsigned int index, unsigned int count)
	{
		Unmap(index * sizeof(TObject), count * sizeof(TObject));
	}
};