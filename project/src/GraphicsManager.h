#pragma once

#include "Exception.h"
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include <vector>

// Exception definitions
DEFINE_EXCEPTION(ShaderCompileException);

class GraphicsManager
{
	std::string m_ShaderPath;
	std::vector<Shader> m_Shaders;
	glm::mat4x4 m_ViewMatrix;
	glm::mat4x4 m_ProjectionMatrix;
	// TODO: Light sources ?? -- figure out from last year's code

	// Shader compilation
	unsigned int CompileShader(unsigned int type, const std::string &name, const std::string &source);
	unsigned int CompileShader(unsigned int type, const std::string &fileName);

public:
	GraphicsManager(std::string shaderPath);
	~GraphicsManager(); // Cleanup shaders, etc.

	void BeginScene(); // Should return RenderContext?
	void EndScene();

	// TODO: Set viewport, etc.?

	// TODO: Shader path
	Shader &CompileShader(const std::string &name, const std::string &code);
	Shader &CompileShader(const std::string &fileName);
};
