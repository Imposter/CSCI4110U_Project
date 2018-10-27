#include "Shader.h"

ShaderVariable::ShaderVariable(GLuint programId, const std::string &name)
	: m_ProgramID(programId), m_Name(name)
{
	m_LocationID = glGetUniformLocation(programId, name.c_str());
}

void ShaderVariable::SetFloat1(float f1) const
{
	glUniform1f(m_LocationID, f1);
}

void ShaderVariable::SetFloat2(float f1, float f2) const
{
	glUniform2f(m_LocationID, f1, f2);
}

void ShaderVariable::SetFloat3(float f1, float f2, float f3) const
{
	glUniform3f(m_LocationID, f1, f2, f3);
}

void ShaderVariable::SetFloat4(float f1, float f2, float f3, float f4) const
{
	glUniform4f(m_LocationID, f1, f2, f3, f4);
}

void ShaderVariable::SetInt1(int i1) const
{
	glUniform1i(m_LocationID, i1);
}

void ShaderVariable::SetInt2(int i1, int i2) const
{
	glUniform2i(m_LocationID, i1, i2);
}

void ShaderVariable::SetInt3(int i1, int i2, int i3) const
{
	glUniform3i(m_LocationID, i1, i2, i3);
}

void ShaderVariable::SetInt4(int i1, int i2, int i3, int i4) const
{
	glUniform4i(m_LocationID, i1, i2, i3, i4);
}

void ShaderVariable::SetUInt1(unsigned int i1) const
{
	glUniform1ui(m_LocationID, i1);
}

void ShaderVariable::SetUInt2(unsigned int i1, unsigned int i2) const
{
	glUniform2ui(m_LocationID, i1, i2);
}

void ShaderVariable::SetUInt3(unsigned int i1, unsigned int i2, unsigned int i3) const
{
	glUniform3ui(m_LocationID, i1, i2, i3);
}

void ShaderVariable::SetUInt4(unsigned int i1, unsigned int i2, unsigned int i3, unsigned int i4) const
{
	glUniform4ui(m_LocationID, i1, i2, i3, i4);
}

void ShaderVariable::SetVecFloat1(unsigned int count, const float *val) const
{
	glUniform1fv(m_LocationID, count, val);
}

void ShaderVariable::SetVecFloat2(unsigned int count, const float *val) const
{
	glUniform2fv(m_LocationID, count, val);
}

void ShaderVariable::SetVecFloat3(unsigned int count, const float *val) const
{
	glUniform3fv(m_LocationID, count, val);
}

void ShaderVariable::SetVecFloat4(unsigned int count, const float *val) const
{
	glUniform4fv(m_LocationID, count, val);
}

void ShaderVariable::SetVecInt1(unsigned int count, const int *val) const
{
	glUniform1iv(m_LocationID, count, val);
}

void ShaderVariable::SetVecInt2(unsigned int count, const int *val) const
{
	glUniform2iv(m_LocationID, count, val);
}

void ShaderVariable::SetVecInt3(unsigned int count, const int *val) const
{
	glUniform3iv(m_LocationID, count, val);
}

void ShaderVariable::SetVecInt4(unsigned int count, const int *val) const
{
	glUniform4iv(m_LocationID, count, val);
}

void ShaderVariable::SetVecUInt1(unsigned int count, const unsigned int *val) const
{
	glUniform1uiv(m_LocationID, count, val);
}

void ShaderVariable::SetVecUInt2(unsigned int count, const unsigned int *val) const
{
	glUniform2uiv(m_LocationID, count, val);
}

void ShaderVariable::SetVecUInt3(unsigned int count, const unsigned int *val) const
{
	glUniform3uiv(m_LocationID, count, val);
}

void ShaderVariable::SetVecUInt4(unsigned int count, const unsigned int *val) const
{
	glUniform4uiv(m_LocationID, count, val);
}

void ShaderVariable::SetVecMatrixFloat2(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix2fv(m_LocationID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat3(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix3fv(m_LocationID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat4(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix4fv(m_LocationID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat2x3(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix2x3fv(m_LocationID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat3x2(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix3x2fv(m_LocationID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat2x4(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix2x4fv(m_LocationID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat4x2(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix4x2fv(m_LocationID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat3x4(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix3x4fv(m_LocationID, count, transpose, val);
}

void ShaderVariable::SetVecMatrixFloat4x3(unsigned int count, bool transpose, const float *val) const
{
	glUniformMatrix4x3fv(m_LocationID, count, transpose, val);
}
