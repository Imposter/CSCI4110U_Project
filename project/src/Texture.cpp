#include "Texture.h"

Texture::Texture(unsigned int width, unsigned int height, Format format, const void *data)
	: m_ID(0), m_Width(width), m_Height(height), m_Format(format), m_WrapModeS(kWrapMode_Repeat), m_WrapModeT(kWrapMode_Repeat), 
	m_FilterModeMin(kFilterMode_NearestMipmapLinear), m_FilterModeMag(kFilterMode_Linear)
{
	// Generate texture buffer
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	// Set wrap mode and filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapModeS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapModeT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterModeMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterModeMag);

	// Create texture
	glTexImage2D(GL_TEXTURE_2D, 0, kFormat_RGBA8, width, height, 0, m_Format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
	// Destroy texture
	glDeleteTextures(1, &m_ID);
}

const GLuint &Texture::GetID() const
{
	return m_ID;
}

unsigned int Texture::GetWidth() const
{
	return m_Width;
}

unsigned int Texture::GetHeight() const
{
	return m_Height;
}

void Texture::GetData(const void *buffer, unsigned int size, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	if (m_Lock.try_lock()) 
	{
		m_Lock.unlock();
		THROW_EXCEPTION(TextureNotLockedException, "Texture must be locked to get data");
	}

	// Check parameters
	if (width > m_Width)
		return;

	if (height > m_Height)
		return;

	// Default parameters
	if (!width) width = m_Width;
	if (!height) height = m_Height;
	
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glGetTextureSubImage(GL_TEXTURE_2D, 0, x, y, 0, width, height, 0, m_Format, GL_UNSIGNED_BYTE, size, &buffer);
}

void Texture::SetData(unsigned int x, unsigned int y, unsigned int width, unsigned int height, const void *data)
{
	if (m_Lock.try_lock()) 
	{
		m_Lock.unlock();
		THROW_EXCEPTION(TextureNotLockedException, "Texture must be locked to set data");
	}
	
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, m_Format, GL_UNSIGNED_BYTE, data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Activate(uint8_t index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Lock()
{
	m_Lock.lock();
}

void Texture::Unlock()
{
	m_Lock.unlock();
}

const Texture::Format &Texture::GetFormat() const
{
	return m_Format;
}

const Texture::WrapMode &Texture::GetWrapModeS() const
{
	return m_WrapModeS;
}

void Texture::SetWrapModeS(const WrapMode &wrapMode)
{
	m_WrapModeS = wrapMode;
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
}

const Texture::WrapMode &Texture::GetWrapModeT() const
{
	return m_WrapModeT;
}

void Texture::SetWrapModeT(const WrapMode &wrapMode)
{
	m_WrapModeT = wrapMode;
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

const Texture::FilterMode &Texture::GetFilterModeMin() const
{
	return m_FilterModeMin;
}

void Texture::SetFilterModeMin(const FilterMode &filterMode)
{
	m_FilterModeMin = filterMode;
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
}

const Texture::FilterMode &Texture::GetFilterModeMag() const
{
	return m_FilterModeMag;
}

void Texture::SetFilterModeMag(const FilterMode &filterMode)
{
	m_FilterModeMag = filterMode;
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
}