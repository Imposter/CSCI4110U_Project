#pragma once

#include <mutex>

class Texture
{
public:
	enum Format
	{
		// ...
		//kFormat_
	};

private:
	unsigned int m_ID;

	unsigned int m_Width;
	unsigned int m_Height;
	const void *m_Data;

	std::mutex m_Lock;

	// TODO: Store properties like: Filtering mode, Mipmaps, etc.

public:
	Texture(); // ...
	~Texture();

	const void *GetData() const;
	const size_t GetSize() const;

	void Lock();
	void Unlock();
};