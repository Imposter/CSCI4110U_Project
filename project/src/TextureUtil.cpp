#include "TextureUtil.h"
#include "Memory.h"
#include "Utility/FileUtil.h"
#include <rapidjson/document.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

enum TextureFormat
{
	kTextureFormat_RGB,
	kTextureFormat_RGBA,
	kTextureFormat_BGR,
	kTextureFormat_BGRA,
};

Texture *LoadTextureFromFile(const std::string &path, const std::string &name)
{
	// Read texture meta data
	const auto metaLines = File::ReadAllLines(path + "/" + name + ".json");
	const auto metaSource = String::Join(metaLines, "\n");

	rapidjson::Document meta;
	meta.Parse(metaSource.c_str());
	if (meta.HasParseError())
		THROW_EXCEPTION(InvalidTextureException, "Meta data parse error: %d", meta.GetParseError());

	// Get file name
	if (!meta.HasMember("name") || !meta["name"].IsString())
		THROW_EXCEPTION(InvalidTextureException, "Meta data invalid name");

	// Get file extension
	if (!meta.HasMember("extension") || !meta["extension"].IsString())
		THROW_EXCEPTION(InvalidTextureException, "Meta data invalid extension");

	// Get format
	if (!meta.HasMember("format") || !meta["format"].IsInt())
		THROW_EXCEPTION(InvalidTextureException, "Meta data invalid format");

	// Build path
	const auto filePath = path + "/" + meta["name"].GetString() + "." + meta["extension"].GetString();

	// Determine format
	Texture::Format format;
	switch (meta["format"].GetInt())
	{
	case kTextureFormat_RGB:
		format = Texture::kFormat_RGB;
		break;
	case kTextureFormat_RGBA:
		format = Texture::kFormat_RGBA;
		break;
	case kTextureFormat_BGR:
		format = Texture::kFormat_BGR;
		break;
	case kTextureFormat_BGRA:
		format = Texture::kFormat_BGRA;
		break;		
	default:
		THROW_EXCEPTION(InvalidTextureException, "Meta data invalid unknown format");
	}

	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	const auto data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	const auto texture = New<Texture>(width, height, format, data);

	stbi_image_free(data);

	return texture;
}

void DestroyTexture(Texture *t)
{
	Delete(t);
}
