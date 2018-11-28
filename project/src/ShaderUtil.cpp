#include "ShaderUtil.h"
#include "Memory.h"
#include "Utility/FileUtil.h"
#include <rapidjson/document.h>

Shader *LoadShaderFromFile(const std::string &path, const std::string &name)
{
	// Read shader meta data
	const auto metaLines = File::ReadAllLines(path + "/" + name + "/meta.json");
	const auto metaSource = String::Join(metaLines, "\n");

	rapidjson::Document meta;
	meta.Parse<rapidjson::kParseCommentsFlag>(metaSource.c_str());
	if (meta.HasParseError())
		THROW_EXCEPTION(InvalidShaderException, "Meta data parse error: %d", meta.GetParseError());

	// Load sources
	std::vector<ShaderSource> sources;

	// Read vertex shaders
	if (!meta.HasMember("vertex") || !meta["vertex"].IsArray())
		THROW_EXCEPTION(InvalidShaderException, "Meta data invalid vertex shaders");

	for (auto &shaderName : meta["vertex"].GetArray())
	{
		if (!shaderName.IsString())
			THROW_EXCEPTION(InvalidShaderException, "Meta data invalid vertex shader");

		const auto lines = File::ReadAllLines(path + "/" + shaderName.GetString() + "/" + shaderName.GetString() + "_vs.glsl");
		sources.push_back({ shaderName.GetString(), kShaderType_Vertex, lines });
	}

	// Read fragment shaders
	if (!meta.HasMember("fragment") || !meta["fragment"].IsArray())
		THROW_EXCEPTION(InvalidShaderException, "Meta data invalid fragment shaders");

	for (auto &shaderName : meta["fragment"].GetArray())
	{
		if (!shaderName.IsString())
			THROW_EXCEPTION(InvalidShaderException, "Meta data invalid fragment shader");

		const auto lines = File::ReadAllLines(path + "/" + shaderName.GetString() + "/" + shaderName.GetString() + "_fs.glsl");
		sources.push_back({ shaderName.GetString(), kShaderType_Fragment, lines });
	}

	return New<Shader>(name, sources);
}

void DestroyShader(Shader *s)
{
	Delete(s);
}
