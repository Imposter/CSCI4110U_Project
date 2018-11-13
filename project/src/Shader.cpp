#include "Shader.h"
#include "Memory.h"
#include <utility>

ShaderVariable::ShaderVariable(GLuint id, std::string name, ShaderVariableType type)
	: m_ID(id), m_Name(std::move(name)), m_Type(type), m_TypeCheck(true)
{
}

GLuint ShaderVariable::GetID() const
{
	return m_ID;
}

const std::string &ShaderVariable::GetName() const
{
	return m_Name;
}

ShaderVariableType ShaderVariable::GetType() const
{
	return m_Type;
}

bool ShaderVariable::IsTypeCheckEnabled() const
{
	return m_TypeCheck;
}

void ShaderVariable::SetTypeCheck(bool enabled)
{
	m_TypeCheck = enabled;
}

void ShaderVariable::SetBool(bool v)
{
	if (m_Type != kShaderVariableType_Bool)
		THROW_EXCEPTION(ShaderVariableTypeMismatchException, "Expected type %d", m_Type);

	glUniform1i(m_ID, v);
}

void ShaderVariable::SetInt(int v)
{
	if (m_TypeCheck && m_Type != kShaderVariableType_Int)
		THROW_EXCEPTION(ShaderVariableTypeMismatchException, "Expected type %d", m_Type);

	glUniform1i(m_ID, v);
}

void ShaderVariable::SetUInt(unsigned int v)
{
	if (m_TypeCheck && m_Type != kShaderVariableType_UInt)
		THROW_EXCEPTION(ShaderVariableTypeMismatchException, "Expected type %d", m_Type);

	glUniform1ui(m_ID, v);
}

void ShaderVariable::SetFloat(float v)
{
	if (m_TypeCheck && m_Type != kShaderVariableType_Float)
		THROW_EXCEPTION(ShaderVariableTypeMismatchException, "Expected type %d", m_Type);

	glUniform1f(m_ID, v);
}

void ShaderVariable::SetDouble(double v)
{
	if (m_TypeCheck && m_Type != kShaderVariableType_Double)
		THROW_EXCEPTION(ShaderVariableTypeMismatchException, "Expected type %d", m_Type);

	glUniform1d(m_ID, v);
}

void ShaderVariable::SetVec2(const glm::vec2 &v)
{
	if (m_TypeCheck && m_Type != kShaderVariableType_Vec2)
		THROW_EXCEPTION(ShaderVariableTypeMismatchException, "Expected type %d", m_Type);

	glUniform2fv(m_ID, 1, &v[0]);
}

void ShaderVariable::SetVec3(const glm::vec3 &v)
{
	if (m_TypeCheck && m_Type != kShaderVariableType_Vec3)
		THROW_EXCEPTION(ShaderVariableTypeMismatchException, "Expected type %d", m_Type);

	glUniform3fv(m_ID, 1, &v[0]);
}

void ShaderVariable::SetVec4(const glm::vec4 &v)
{
	if (m_TypeCheck && m_Type != kShaderVariableType_Vec4)
		THROW_EXCEPTION(ShaderVariableTypeMismatchException, "Expected type %d", m_Type);

	glUniform4fv(m_ID, 1, &v[0]);
}

void ShaderVariable::SetMat4(const glm::mat4 &v, bool transpose)
{
	if (m_TypeCheck && m_Type != kShaderVariableType_Mat4)
		THROW_EXCEPTION(ShaderVariableTypeMismatchException, "Expected type %d", m_Type);

	glUniformMatrix4fv(m_ID, 1, transpose, &v[0][0]);
}

GLuint Shader::compileShader(GLenum type, const void *source)
{
	// Create a shader with the specified source code
	const auto shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, reinterpret_cast<const GLchar **>(&source), nullptr);

	// Compile the shader
	glCompileShader(shaderId);

	// Check if there were any compilation errors
	GLint result = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint errorLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorLength);

		std::string errorMessage;
		errorMessage.resize(errorLength);
		glGetShaderInfoLog(shaderId, errorLength, &errorLength, const_cast<char *>(errorMessage.c_str()));

		glDeleteShader(shaderId);

		THROW_EXCEPTION(ShaderCompileException, "Shader compilation failed: %s", errorMessage.c_str());
	}

	return shaderId;
}

Shader::Shader(std::string name, std::vector<ShaderSource> sources)
	: m_Name(std::move(name)), m_Sources(std::move(sources)), m_ID(0), m_Compiled(false)
{
}

Shader::~Shader()
{
	// Delete vars
	for (auto &v : m_Variables)
		Delete(v);
	m_Variables.clear();

	if (m_Compiled)
	{
		glDeleteProgram(m_ID);
		m_Compiled = false;
	}
}

const std::string &Shader::GetName() const
{
	return m_Name;
}

GLuint Shader::GetID() const
{
	return m_ID;
}

ShaderVariable *Shader::GetVariable(const std::string &name)
{
	// Find variable
	for (auto &var : m_Variables)
	{
		if (var->GetName() == name)
			return var;
	}

	THROW_EXCEPTION(ShaderVariableNotFoundException, "Variable %s not found", name.c_str());
}

std::vector<ShaderVariable *> Shader::GetVariables() const
{
	return m_Variables;
}

void Shader::Compile()
{
	// Compile shaders
	std::vector<GLuint> shaderIds;

	for (auto &source : m_Sources)
	{
		const auto code = String::Join(source.Code, "\n");
		const auto shaderId = compileShader(source.Type, code.c_str());
		shaderIds.push_back(shaderId);
	}
	
	// Create and link the shaders into a program
	m_ID = glCreateProgram();
	for (auto &id : shaderIds)
		glAttachShader(m_ID, id);
	glLinkProgram(m_ID);
	glValidateProgram(m_ID);

	// Check if there were any linking errors
	GLint result = 0;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint errorLength = 0;
		glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &errorLength);

		std::string errorMessage;
		errorMessage.resize(errorLength);
		glGetProgramInfoLog(m_ID, errorLength, &errorLength, const_cast<char *>(errorMessage.c_str()));

		// Delete shaders
		for (auto &id : shaderIds)
		{
			glDetachShader(m_ID, id);
			glDeleteShader(id);
		}

		// Delete program
		glDeleteProgram(m_ID);

		THROW_EXCEPTION(ShaderLinkException, "Shader linking failed: %s", errorMessage.c_str());
	}

	// Delete shaders
	for (auto &id : shaderIds)
	{
		glDetachShader(m_ID, id);
		glDeleteShader(id);
	}

	// Get max length of uniform name
	GLint maxNameLength;
	glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

	// Get uniform count
	GLint count;
	glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &count);

	for (auto i = 0; i < count; i++)
	{
		// Get uniform information
		std::string varName;
		varName.resize(maxNameLength);
		GLsizei varNameLength;

		GLint size;
		GLenum type;

		glGetActiveUniform(m_ID, static_cast<GLuint>(i), maxNameLength, &varNameLength, &size, &type, const_cast<char *>(varName.c_str()));
		varName.resize(varNameLength);

		// Get uniform location
		const auto location = glGetUniformLocation(m_ID, varName.c_str());

		// Store
		m_Variables.push_back(New<ShaderVariable>(location, varName, static_cast<ShaderVariableType>(type)));
	}

	// Set as compiled
	m_Compiled = true;
}

void Shader::Use()
{
	if (!m_Compiled)
		THROW_EXCEPTION(ShaderNotCompiledException, "Cannot use uncompiled shader %s", m_Name.c_str());

	glUseProgram(m_ID);
}
