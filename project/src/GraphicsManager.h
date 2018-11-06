#pragma once

#include "Texture.h"
#include "Shader.h"
#include <map>

class GraphicsManager
{
	std::string m_ShaderPath;
	std::map<std::string, Shader *> m_Shaders;

public:
	GraphicsManager(std::string shaderPath);
	~GraphicsManager();	

	// NOTE: These return unsigned int addresses to GL pointers, RenderTexture, etc. access these?
	// Or these return RenderTarget which takes unsigned int id as constructor param
	// TODO: CreateRenderTarget/SetRenderTarget

	// TODO: Add clear depth/color here

	// TODO: RenderTarget...

	void Clear();


	// TODO: Use camera func, or so?
	void BeginScene(); // Should return RenderContext?
	void EndScene();

	Shader *GetShader(const std::string &name);
}; 
// TODO/NOTE: We could still remove this entirely and pass shaders manually to Node::Render, since we're calling
// TODO/NOTE: OpenGL functions directly anyways
