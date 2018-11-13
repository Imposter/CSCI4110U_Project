#pragma once

#include "Shader.h"
#include "Texture.h"
#include <map>

class GraphicsManager
{
	std::string m_DataPath;
	std::map<std::string, Shader *> m_Shaders;
	std::map<std::string, Texture *> m_Textures;
	// TODO: Materials

public:
	GraphicsManager(std::string dataPath);
	~GraphicsManager();

	// No copying/moving
	GraphicsManager(const GraphicsManager &) = delete;
	GraphicsManager &operator=(const GraphicsManager &) = delete;

	GraphicsManager(const GraphicsManager &&) = delete;
	GraphicsManager &operator=(const GraphicsManager &&) = delete;

	Shader *GetShader(const std::string &name);
	Texture *GetTexture(const std::string &name);
}; 