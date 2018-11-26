#pragma once

#include "Utility/Exception.h"
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

// Exception definitions
DEFINE_EXCEPTION(ShaderCompileException);
DEFINE_EXCEPTION(ShaderLinkException);
DEFINE_EXCEPTION(ShaderNotCompiledException);
DEFINE_EXCEPTION(ShaderVariableNotFoundException);
DEFINE_EXCEPTION(ShaderVariableTypeMismatchException);

#define SHADER_DEFINE_VARIABLE(name) static const char *kShaderVar_ ## name =  "u_" #name

// Vars
SHADER_DEFINE_VARIABLE(Transform);

enum ShaderVariableType
{
	kShaderVariableType_Unknown,

	kShaderVariableType_Bool = GL_BOOL,

	kShaderVariableType_Int = GL_INT,
	kShaderVariableType_UInt = GL_UNSIGNED_INT,

	kShaderVariableType_Float = GL_FLOAT,
	kShaderVariableType_Double = GL_DOUBLE,

	kShaderVariableType_Vec2 = GL_FLOAT_VEC2,
	kShaderVariableType_Vec3 = GL_FLOAT_VEC3,
	kShaderVariableType_Vec4 = GL_FLOAT_VEC4,

	kShaderVariableType_Mat4 = GL_FLOAT_MAT4,

	// Internal
	kShaderVariableType_Sampler2D = GL_SAMPLER_2D,
};

class ShaderVariable
{
	GLuint m_ID;
	std::string m_Name;
	ShaderVariableType m_Type;
	bool m_TypeCheck;

public:
	ShaderVariable(GLuint id, std::string name, ShaderVariableType type);

	GLuint GetID() const;
	const std::string &GetName() const;
	ShaderVariableType GetType() const;

	bool IsTypeCheckEnabled() const;
	void SetTypeCheck(bool enabled);

	void SetBool(bool v);
	void SetInt(int v);
	void SetUInt(unsigned int v);
	void SetFloat(float v);
	void SetDouble(double v);
	void SetVec2(const glm::vec2 &v);
	void SetVec3(const glm::vec3 &v);
	void SetVec4(const glm::vec4 &v);
	void SetMat4(const glm::mat4 &m, bool transpose = false);
};

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
	std::vector<ShaderVariable *> m_Variables;

	GLuint m_ID;
	bool m_Compiled;

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
	GLuint GetID() const;

	ShaderVariable *GetVariable(const std::string &name);
	std::vector<ShaderVariable *> GetVariables() const;

	void Compile();
	void Use();
};