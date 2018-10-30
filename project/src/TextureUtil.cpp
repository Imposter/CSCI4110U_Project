#include "TextureUtil.h"
#include "Memory.h"
#include "Utility/FileUtil.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture *LoadTextureFromFile(const std::string &fileName, Texture::Format format)
{
	if (!File::Exists(fileName))
		THROW_EXCEPTION(FileNotFoundException, "Texture file \"%s\" does not exist", fileName.c_str());

	int width, height, channels;
	const auto data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);

	const auto texture = New<Texture>(width, height, format, data);

	stbi_image_free(data);

	return texture;
}