#pragma once

#include "Utility/Exception.h"
#include <GL/glew.h>
#include <mutex>

DEFINE_EXCEPTION(TextureNotLockedException);

class Texture
{
public:
	enum Format
	{
		// Internal
		kFormat_RGB8 = GL_RGB8,
		kFormat_RGBA8 = GL_RGBA8,

		kFormat_RGB = GL_RGB,
		kFormat_RGBA = GL_RGBA,
		kFormat_BGR = GL_BGR,
		kFormat_BGRA = GL_BGRA

	};

	enum WrapMode
	{
		kWrapMode_Repeat = GL_REPEAT,
		kWrapMode_MirrorRepeat = GL_MIRRORED_REPEAT,
		kWrapMode_ClampToEdge = GL_CLAMP_TO_EDGE,
		kWrapMode_ClampToBorder = GL_CLAMP_TO_BORDER
	};

	enum FilterMode 
	{
		kFilterMode_Nearest = GL_NEAREST,
		kFilterMode_Linear = GL_LINEAR,

		// Used for min, not for mag
		kFilterMode_NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		kFilterMode_NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		kFilterMode_LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		kFilterMode_LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
	};

private:
	GLuint m_ID;

	unsigned int m_Width;
	unsigned int m_Height;

	std::mutex m_Lock;
	
	Format m_Format;
	WrapMode m_WrapModeS;
	WrapMode m_WrapModeT;
	FilterMode m_FilterModeMin;
	FilterMode m_FilterModeMag;

public:
	Texture(unsigned int width, unsigned int height, Format format = kFormat_RGBA, const void *data = nullptr);
	~Texture();
	
	Texture(const Texture &) = delete;
	Texture &operator=(const Texture &) = delete;

	Texture(const Texture &&) = delete;
	Texture &operator=(const Texture &&) = delete;

	const GLuint &GetID() const;
	
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	
	void GetData(const void *buffer, unsigned int size, unsigned int x = 0, unsigned int y = 0, unsigned int width = 0, unsigned int height = 0) const;
	void SetData(unsigned int x, unsigned int y, unsigned int width, unsigned int height, const void *data);

	void Bind();

	void Lock();
	void Unlock();

	const Format &GetFormat() const;
	
	const WrapMode &GetWrapModeS() const;
	void SetWrapModeS(const WrapMode &wrapMode);
	const WrapMode &GetWrapModeT() const;
	void SetWrapModeT(const WrapMode &wrapMode);
	
	const FilterMode &GetFilterModeMin() const;
	void SetFilterModeMin(const FilterMode &filterMode);
	const FilterMode &GetFilterModeMag() const;
	void SetFilterModeMag(const FilterMode &filterMode);
};