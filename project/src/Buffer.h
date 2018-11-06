#pragma once

#include "Utility/Exception.h"
#include <cstdint>
#include <GL/glew.h>

DEFINE_EXCEPTION(BufferMapException);

// TODO/NOTE: We could also use this in textures, which would allow us to monitor video memory too since we'd be using it for everything
class Buffer
{
public:
	enum Target
	{
		kTarget_None = 0,

		kTarget_ArrayBuffer = GL_ARRAY_BUFFER,
		kTarget_ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
		kTarget_QueryBuffer = GL_QUERY_BUFFER,
		kTarget_TextureBuffer = GL_TEXTURE_BUFFER,

		kTarget_CopyReadBuffer = GL_COPY_READ_BUFFER,
		kTarget_CopyWriteBuffer = GL_COPY_WRITE_BUFFER,
	};

	enum Access
	{
		kAccess_None = 0,

		kAccess_Read = GL_MAP_READ_BIT,
		kAccess_Write = GL_MAP_WRITE_BIT,
		kAccess_ReadWrite = kAccess_Read | kAccess_Write
	};

protected:
	GLuint m_ID;
	Target m_Target;
	size_t m_Size;
	bool m_Dynamic;

	bool m_Mapped;
	Access m_MappedAccess;

private:
	void init(const void *data = nullptr);

public:
	Buffer(Target target, size_t size, const void *data = nullptr, bool dynamic = false);
	~Buffer();

	Buffer(const Buffer &copy);
	Buffer &operator=(const Buffer &copy);

	// No moving
	Buffer(const Buffer &&) = delete;
	Buffer &operator=(const Buffer &&) = delete;

	const GLuint &GetID() const;
	Target GetTarget() const;
	size_t GetSize() const;
	void SetSize(size_t size);

	void Bind(Target target = kTarget_None);
	void Copy(const Buffer &buffer);

	const void *Map(unsigned int offset, size_t size, Access access = kAccess_ReadWrite);
	void Unmap(unsigned int offset, size_t size);

	template<typename TObject>
	const TObject *Map(unsigned int index, unsigned int count, Access access = kAccess_ReadWrite)
	{
		return (TObject *)Map(index * sizeof(TObject), count * sizeof(TObject), access);
	}

	template<typename TObject>
	void Unmap(unsigned int index, unsigned int count)
	{
		Unmap(index * sizeof(TObject), count * sizeof(TObject));
	}
};