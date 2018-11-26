#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include <map>

class GraphicsManager
{
	std::string m_DataPath;
	std::map<std::string, Shader *> m_Shaders;
	std::map<std::string, Texture *> m_Textures;

	Shader *m_ActiveShader;
	VertexArray *m_ActiveVertexArray;
	VertexBuffer<void> *m_ActiveVertexBuffer;
	IndexBuffer *m_ActiveIndexBuffer;

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

	void UseShader(Shader *shader);

	void Bind(VertexArray *va);
	void Bind(VertexBuffer<void> *vb);
	void Bind(IndexBuffer *ib);

	template<typename TVertex>
	void Bind(VertexBuffer<TVertex> *vb)
	{
		Bind(reinterpret_cast<VertexBuffer<void> *>(vb));
	}
}; 