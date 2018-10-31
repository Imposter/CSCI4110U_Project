#include "GraphicsManager.h"
#include "ShaderUtil.h"
#include <utility>

GraphicsManager::GraphicsManager(std::string shaderPath)
	: m_ShaderPath(std::move(shaderPath))
{
}

GraphicsManager::~GraphicsManager()
{
	// Unload shader programs
	for (auto &pair : m_Shaders)
		DestroyShader(pair.second);

	m_Shaders.clear();
}

Shader *GraphicsManager::GetShader(const std::string &name)
{
	// Check if it is already loaded
	if (m_Shaders.find(name) != m_Shaders.end())
		return m_Shaders[name];

	// Compile shader
	const auto shader = LoadShaderFromFile(m_ShaderPath, name);
	shader->Compile();

	// Store shader
	m_Shaders.emplace(name, shader);

	return shader;
}