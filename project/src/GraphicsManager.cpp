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
	std::map<std::string, Shader *>::iterator it;
	if ((it = m_Shaders.find(name)) != m_Shaders.end())
		return it->second;

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
	std::map<std::string, Texture *>::iterator it;
	if ((it = m_Textures.find(name)) != m_Textures.end())
		return it->second;

	// Load texture
	const auto texture = LoadTextureFromFile(m_DataPath + "/textures", name);

	// Store texture
	m_Textures.emplace(name, texture);

	return texture;
}

// TODO: Integrate
void GraphicsManager::UseShader(Shader *shader)
{
	if (shader == m_ActiveShader)
		return;

	m_ActiveShader = shader;
	shader->Use();
}

void GraphicsManager::Bind(VertexArray *va)
{
	if (va == m_ActiveVertexArray)
		return;

	m_ActiveVertexArray = va;
	va->Bind();
}

void GraphicsManager::Bind(VertexBuffer<void> *vb)
{
	if (vb == m_ActiveVertexBuffer)
		return;

	m_ActiveVertexBuffer = vb;
	vb->Bind();
}

void GraphicsManager::Bind(IndexBuffer *ib)
{
	if (ib == m_ActiveIndexBuffer)
		return;

	m_ActiveIndexBuffer = ib;
	ib->Bind();
}
