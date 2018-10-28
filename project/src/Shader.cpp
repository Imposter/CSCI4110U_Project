#include "Shader.h"
#include "Log.h"
#include <utility>

ShaderVariable::ShaderVariable(GLuint id, std::string name)
	: m_ID(id), m_Name(std::move(name))
{
}

const GLuint &ShaderVariable::GetID() const
{
	return m_ID;
}

const std::string &ShaderVariable::GetName() const
{
	return m_Name;
}

void ShaderVariable::SetFloat1(float f1) const
{
	glUniform1f(m_ID, f1);
}

void ShaderVariable::SetFloat2(float f1, float f2) const
{
	glUniform2f(m_ID, f1, f2);
}

void ShaderVariable::SetFloat3(float f1, float f2, float f3) const
{
	glUniform3f(m_ID, f1, f2, f3);
}

void ShaderVariable::SetFloat4(float f1, float f2, float f3, float f4) const
{
	glUniform4f(m_ID, f1, f2, f3, f4);
}

void ShaderVariable::SetInt1(int i1) const
{
	glUniform1i(m_ID, i1);
}

void ShaderVariable::SetInt2(int i1, int i2) const
{
	glUniform2i(m_ID, i1, i2);
}

void ShaderVariable::SetInt3(int i1, int i2, int i3) const
{
	glUniform3i(m_ID, i1, i2, i3);
}

void ShaderVariable::SetInt4(int i1, int i2, int i3, int i4) const
{
	glUniform4i(m_ID, i1, i2, i3, i4);
}

void ShaderVariable::SetUInt1(unsigned int i1) const
{
	glUniform1ui(m_ID, i1);
}

void ShaderVariable::SetUInt2(unsigned int i1, unsigned int i2) const
{
	glUniform2ui(m_ID, i1, i2);
}

void ShaderVariable::SetUInt3(unsigned int i1, unsigned int i2, unsigned int i3) const
{
	glUniform3ui(m_ID, i1, i2, i3);
}

void ShaderVariable::SetUInt4(unsigned int i1, unsigned int i2, unsigned int i3, unsigned int i4) const
{
	glUniform4ui(m_ID, i1, i2, i3, i4);
}

void ShaderVariable::SetVecFloat1(unsigned int count, const float *val) const
{
	glUniform1fv(m_ID, count, val);
}

void ShaderVariable::SetVecFloat2(unsigned int count, const float *val) const
{
	glUniform2fv(m_ID, count, val);
}

void ShaderVariable::SetVecFloat3(unsigned int count, const float *val) const
{
	glUniform3fv(m_ID, count, val);
}

void ShaderVariable::SetVecFloat4(unsigned int count, const float *val) const
{
	glUniform4fv(m_ID, count, val);
}

void ShaderVariable::SetVecInt1(unsigned int count, const int *val) const
{
	glUniform1iv(m_ID, count, val);
}

void ShaderVariable::SetVecInt2(unsigned int count, const int *val) const
{
	glUniform2iv(m_ID, count, val);
}

void ShaderVariable::SetVecInt3(unsigned int count, const int *val) const
{
	glUniform3iv(m_ID, count, val);
}

void ShaderVariable::SetVecInt4(unsigned int count, const int *val) const
{
	glUniform4iv(m_ID, count, val);
}

void ShaderVariable::SetVecUInt1(unsigned int count, const unsigned int *val) const
{
	glUniform1uiv(m_ID, count, val);
}

void ShaderVariable::SetVecUInt2(unsigned int count, const unsigned int *val) const
{
	glUniform2uiv(m_ID, count, val);
}

void ShaderVariable::SetVecUInt3(unsigned int count, const unsigned int *val) const
{
	glUniform3uiv(m_ID, count, val);
}

void ShaderVariable::SetVecUInt4(unsigned int count, const unsigned int *val) const
{
	glUniform4uiv(m_ID, count, val);
}

void ShaderVariable::SetVecMatrixFloat2(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix2fv(m_ID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat3(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix3fv(m_ID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat4(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix4fv(m_ID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat2x3(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix2x3fv(m_ID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat3x2(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix3x2fv(m_ID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat2x4(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix2x4fv(m_ID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat4x2(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix4x2fv(m_ID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat3x4(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix3x4fv(m_ID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat4x3(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix4x3fv(m_ID, count, transpose, val);
}

Shader::Shader(std::string name, GLuint id)
	: m_Name(std::move(name)), m_ID(id)
{
	// Get max length of uniform name GL_ACTIVE_UNIFORM_MAX_LENGTH
	GLint maxNameLength;
	glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

	// Get uniform count
	GLint count;
	glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &count);

	LOG_TRACE("Shader", "Shader %s has %d variables", name.c_str(), count);

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

		LOG_TRACE("Shader", "- Variable %s of type %d", varName.c_str(), type);

		// Store
		m_Variables.push_back(new ShaderVariable(i, varName));
	}
}

Shader::~Shader()
{
	for (auto &var : m_Variables)
		delete var;

	m_Variables.clear();
}

const std::string &Shader::GetName() const
{
	return m_Name;
}

const GLuint &Shader::GetID() const
{
	return m_ID;
}

ShaderVariable *Shader::GetVariable(std::string name) const
{
	for (auto &var : m_Variables)
	{
		if (var->GetName() == name)
			return var;
	}

	THROW_EXCEPTION(ShaderVariableNotFoundException, "Variable %s not found", name.c_str());
}

void Shader::Apply()
{
	glUseProgram(m_ID);
}
