#include "GraphicsManager.h"
#include <fstream>
#include <sstream>
#include <utility>

GraphicsManager::GraphicsManager(std::string shaderPath)
	: m_ShaderPath(std::move(shaderPath))
{
}

GraphicsManager::~GraphicsManager()
{
	// Unload shader programs
	for (auto &shader : m_Shaders)
	{
		glDeleteProgram(shader->GetID());
		delete shader;
	}

	m_Shaders.clear();
}

Shader *GraphicsManager::CompileShader(const std::string &name)
{
	// Create and compile a shader for each
	const auto vShaderId = compileShader(GL_VERTEX_SHADER, m_ShaderPath + "/" + name + "_vs.glsl");
	const auto fShaderId = compileShader(GL_FRAGMENT_SHADER, m_ShaderPath + "/" + name + "_fs.glsl");

	// Create and link the shaders into a program
	const auto programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);
	glLinkProgram(programId);
	glValidateProgram(programId);

	// Delete the shaders
	glDetachShader(programId, vShaderId);
	glDetachShader(programId, fShaderId);
	glDeleteShader(vShaderId);
	glDeleteShader(fShaderId);

	// Store shader
	const auto shader = new Shader(name, programId);
	m_Shaders.push_back(shader);

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
	std::ifstream file(fileName);
	if (!file.is_open())
		THROW_EXCEPTION(ShaderCompileException, "File not found: %s", fileName.c_str());

	std::string shaderSource;
	std::string line;
	while (getline(file, line)) 
	{
		shaderSource.append(line);
		shaderSource.append("\n");
	}

	return compileShader(type, reinterpret_cast<const void *>(shaderSource.c_str()));
}
