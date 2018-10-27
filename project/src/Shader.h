#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

class ShaderVariable
{
	GLuint m_ProgramID;
	GLuint m_LocationID;
	std::string m_Name;

public:
	ShaderVariable(GLuint programId, const std::string &name);

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

enum ShaderType
{
	kShaderType_Vertex,
	kShaderType_Fragment
};

class GraphicsManager; // ?
class Shader // GraphicsManager should create shader
{
	std::string m_Name;
	ShaderType m_Type;
	GLuint m_ProgramID;
	std::vector<ShaderVariable> m_Variables; // https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade
	// TODO: Store buffers?
	//GraphicsManager m_Graphics;

public:
	Shader(); // TODO: ...

	void Apply();
	ShaderVariable &GetVariable(std::string name) const;
};