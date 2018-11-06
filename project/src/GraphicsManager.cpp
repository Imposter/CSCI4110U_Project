#include "GraphicsManager.h"
#include "ShaderUtil.h"
#include "TextureUtil.h"
#include <utility>

GraphicsManager::GraphicsManager(std::string dataPath)
	: m_DataPath(std::move(dataPath))
{
}

GraphicsManager::~GraphicsManager()
{
	// Unload shader programs
	for (auto &pair : m_Shaders)
		DestroyShader(pair.second);

	m_Shaders.clear();

	// Destroy textures
	for (auto &pair : m_Textures)
		DestroyTexture(pair.second);

	m_Textures.clear();
}

Shader *GraphicsManager::GetShader(const std::string &name)
{
	// Check if it is already loaded
	if (m_Shaders.find(name) != m_Shaders.end())
		return m_Shaders[name];

	// Compile shader
	const auto shader = LoadShaderFromFile(m_DataPath + "/shaders", name);
	shader->Compile();

	// Store shader
	m_Shaders.emplace(name, shader);

	return shader;
}

Texture *GraphicsManager::GetTexture(const std::string &name)
{
	// Check if it is already loaded
	if (m_Textures.find(name) != m_Textures.end())
		return m_Textures[name];

	// Load texture
	const auto texture = LoadTextureFromFile(m_DataPath + "/textures", name);

	// Store texture
	m_Textures.emplace(name, texture);

	return texture;
}