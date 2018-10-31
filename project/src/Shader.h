#pragma once

#include "Utility/Exception.h"
#include <string>
#include <vector>
#include <GL/glew.h>

// Exception definitions
DEFINE_EXCEPTION(ShaderCompileException);
DEFINE_EXCEPTION(ShaderNotCompiledException);

class ShaderVariable
{
	GLuint m_ID;
	std::string m_Name;

public:
	ShaderVariable(GLuint id, std::string name);

	const GLuint &GetID() const;
	const std::string &GetName() const;

	void SetFloat1(float f1) const;
	void SetFloat2(float f1, float f2) const;
	void SetFloat3(float f1, float f2, float f3) const;
	void SetFloat4(float f1, float f2, float f3, float f4) const;

	void SetInt1(int i1) const;
	void SetInt2(int i1, int i2) const;
	void SetInt3(int i1, int i2, int i3) const;
	void SetInt4(int i1, int i2, int i3, int i4) const;

	void SetUInt1(unsigned int i1) const;
	void SetUInt2(unsigned int i1, unsigned int i2) const;
	void SetUInt3(unsigned int i1, unsigned int i2, unsigned int i3) const;
	void SetUInt4(unsigned int i1, unsigned int i2, unsigned int i3, unsigned int i4) const;

	void SetVecFloat1(unsigned int count, const float *val) const;
	void SetVecFloat2(unsigned int count, const float *val) const;
	void SetVecFloat3(unsigned int count, const float *val) const;
	void SetVecFloat4(unsigned int count, const float *val) const;

	void SetVecInt1(unsigned int count, const int *val) const;
	void SetVecInt2(unsigned int count, const int *val) const;
	void SetVecInt3(unsigned int count, const int *val) const;
	void SetVecInt4(unsigned int count, const int *val) const;

	void SetVecUInt1(unsigned int count, const unsigned int *val) const;
	void SetVecUInt2(unsigned int count, const unsigned int *val) const;
	void SetVecUInt3(unsigned int count, const unsigned int *val) const;
	void SetVecUInt4(unsigned int count, const unsigned int *val) const;

	void SetVecMatrixFloat2(unsigned int count, bool transpose, const float *val) const;
	void SetVecMatrixFloat3(unsigned int count, bool transpose, const float *val) const;
	void SetVecMatrixFloat4(unsigned int count, bool transpose, const float *val) const;
	void SetVecMatrixFloat2x3(unsigned int count, bool transpose, const float *val) const;
	void SetVecMatrixFloat3x2(unsigned int count, bool transpose, const float *val) const;
	void SetVecMatrixFloat2x4(unsigned int count, bool transpose, const float *val) const;
	void SetVecMatrixFloat4x2(unsigned int count, bool transpose, const float *val) const;
	void SetVecMatrixFloat3x4(unsigned int count, bool transpose, const float *val) const;
	void SetVecMatrixFloat4x3(unsigned int count, bool transpose, const float *val) const;
};

DEFINE_EXCEPTION(ShaderVariableNotFoundException);

enum ShaderType
{
	kShaderType_Fragment = GL_FRAGMENT_SHADER,
	kShaderType_Vertex = GL_VERTEX_SHADER
};

struct ShaderSource
{
	std::string Name;
	ShaderType Type;
	std::vector<std::string> Code;
};

class Shader
{
	std::string m_Name;
	std::vector<ShaderSource> m_Sources;

	GLuint m_ID;
	bool m_Compiled;
	std::vector<ShaderVariable *> m_Variables;

	// Shader compilation
	static GLuint compileShader(GLenum type, const void *source);

public:
	Shader(std::string name, std::vector<ShaderSource> sources);
	~Shader();

	Shader(const Shader &) = delete;
	Shader &operator=(const Shader &) = delete;

	Shader(const Shader &&) = delete;
	Shader &operator=(const Shader &&) = delete;

	const std::string &GetName() const;
	const GLuint &GetID() const;

	ShaderVariable *GetVariable(const std::string &name) const;

	void Compile();
	void Use();
};