#include "GraphicsManager.h"
#include "Log.h"

GraphicsManager::GraphicsManager(std::string shaderPath)
	: m_ShaderPath(shaderPath)
{
}

unsigned GraphicsManager::CompileShader(unsigned type, const std::string &name, const std::string &source)
{
	const auto sourceCode = source.c_str();

	// Create a shader with the specified source code
	const auto shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &sourceCode, nullptr);

	// Compile the shader
	glCompileShader(shaderId);

	// Check if there were any compilation errors
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int errorLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorLength);
		const auto errorMessage = new char[errorLength];

		glGetShaderInfoLog(shaderId, errorLength, &errorLength, errorMessage);
		LOG_ERROR("GraphicsManager", "Shader compilation failed %s", errorMessage);

		delete[] errorMessage;

		glDeleteShader(shaderId);

		return 0;
	}

	return shaderId;
}

unsigned GraphicsManager::CompileShader(unsigned type, const std::string &fileName)
{

}
