#include "GraphicsManager.h"
#include "Utility/FileUtil.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <utility>

GraphicsManager::GraphicsManager(std::string shaderPath)
	: m_ShaderPath(std::move(shaderPath))
{
}

GraphicsManager::~GraphicsManager()
{
	// Unload shader programs
	for (auto &pair : m_Shaders)
	{
		glDeleteProgram(pair.second->GetID());
		delete pair.second;
	}

	m_Shaders.clear();
}

Shader *GraphicsManager::GetShader(const std::string &name)
{
	// Check if it is already loaded
	if (m_Shaders.find(name) != m_Shaders.end())
		return m_Shaders[name];

	// Read shader meta data
	const auto metaLines = File::ReadAllLines(m_ShaderPath + "/" + name + ".json");
	const auto metaSource = String::Join(metaLines, "\n");

	rapidjson::Document meta;
	meta.Parse(metaSource.c_str());
	if (meta.HasParseError())
		THROW_EXCEPTION(InvalidShaderException, "Meta data parse error: %d", meta.GetParseError());

	// Load shaders
	std::vector<unsigned int> shaderIds;

	// Compile vertex shaders
	if (!meta.HasMember("vertex") || !meta["vertex"].IsArray())
		THROW_EXCEPTION(InvalidShaderException, "Meta data invalid vertex shaders");

	for (auto &shaderName : meta["vertex"].GetArray())
	{
		if (!shaderName.IsString())
			THROW_EXCEPTION(InvalidShaderException, "Meta data invalid vertex shader");

		const auto shaderId = compileShader(GL_VERTEX_SHADER, m_ShaderPath + "/" + shaderName.GetString() + "_vs.glsl");
		shaderIds.push_back(shaderId);
	}

	// Compile fragment shaders
	if (!meta.HasMember("fragment") || !meta["fragment"].IsArray())
		THROW_EXCEPTION(InvalidShaderException, "Meta data invalid fragment shaders");

	for (auto &shaderName : meta["fragment"].GetArray())
	{
		if (!shaderName.IsString())
			THROW_EXCEPTION(InvalidShaderException, "Meta data invalid fragment shader");

		const auto shaderId = compileShader(GL_FRAGMENT_SHADER, m_ShaderPath + "/" + shaderName.GetString() + "_fs.glsl");
		shaderIds.push_back(shaderId);
	}

	// Create and link the shaders into a program
	const auto programId = glCreateProgram();
	for (auto &id : shaderIds)
		glAttachShader(programId, id);
	glLinkProgram(programId);
	glValidateProgram(programId);

	// Delete shaders
	for (auto &id : shaderIds)
	{
		glDetachShader(programId, id);
		glDeleteShader(id);
	}

	// Store shader
	const auto shader = new Shader(name, programId);
	m_Shaders.emplace(name, shader);

	return shader;
}

unsigned int GraphicsManager::compileShader(unsigned int type, const void *source)
{
	// Create a shader with the specified source code
	const auto shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, reinterpret_cast<const GLchar **>(&source), nullptr);

	// Compile the shader
	glCompileShader(shaderId);

	// Check if there were any compilation errors
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) 
	{
		int errorLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorLength);

		std::string errorMessage;
		errorMessage.resize(errorLength);
		glGetShaderInfoLog(shaderId, errorLength, &errorLength, const_cast<char *>(errorMessage.c_str()));

		glDeleteShader(shaderId);

		THROW_EXCEPTION(ShaderCompileException, "Shader compilation failed: %s", errorMessage.c_str());
	}

	return shaderId;
}

unsigned int GraphicsManager::compileShader(unsigned int type, const std::string &fileName)
{
	const auto lines = File::ReadAllLines(fileName);
	const auto source = String::Join(lines, "\n");

	return compileShader(type, reinterpret_cast<const void *>(source.c_str()));
}