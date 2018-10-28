#pragma once

#include "Shader.h"
#include "Utility/Exception.h"
#include <glm/mat4x4.hpp>
#include <memory>
#include <map>

// Exception definitions
DEFINE_EXCEPTION(InvalidShaderException);
DEFINE_EXCEPTION(ShaderCompileException);

class GraphicsManager
{
	std::string m_ShaderPath;
	std::map<std::string, Shader *> m_Shaders;
	glm::mat4x4 m_ViewMatrix; // NOTE: These are set by camera -- GameObject and Components are going to be different classes that do different things (Component for Logic, GameObject for game object like mesh, cam etc)
	glm::mat4x4 m_ProjectionMatrix;
	// TODO: Light sources ?? -- figure out from last year's code

	// Shader compilation
	static unsigned int compileShader(unsigned int type, const void *source);
	static unsigned int compileShader(unsigned int type, const std::string &fileName);

public:
	GraphicsManager(std::string shaderPath);
	~GraphicsManager(); // Cleanup shaders, etc.

	void BeginScene(); // Should return RenderContext?
	void EndScene();

	// TODO: Set viewport, etc.? -- we need to merge last year code!

	Shader *GetShader(const std::string &name);
};
