#pragma once

#include "Texture.h"
#include "Shader.h"
#include "GraphicsManager.h"

DEFINE_EXCEPTION(MaterialVariableNotFoundException);
DEFINE_EXCEPTION(MaterialTextureNotFoundException);
DEFINE_EXCEPTION(MaterialUnsupportedTypeException);

#define MATERIAL_KEY_NAME "u_Material"
#define MATERIAL_DEFINE_VARIABLE(name) static const char *kMaterialVar_ ## name =  MATERIAL_KEY_NAME "." #name
#define MATERIAL_LOCAL_NAME(name) #name

// Default vars
MATERIAL_DEFINE_VARIABLE(Ambient);
MATERIAL_DEFINE_VARIABLE(Diffuse);
MATERIAL_DEFINE_VARIABLE(Specular);

MATERIAL_DEFINE_VARIABLE(TextureAmbient);
MATERIAL_DEFINE_VARIABLE(TextureAmbientEnabled);
MATERIAL_DEFINE_VARIABLE(TextureDiffuse);
MATERIAL_DEFINE_VARIABLE(TextureDiffuseEnabled);
MATERIAL_DEFINE_VARIABLE(TextureSpecular);
MATERIAL_DEFINE_VARIABLE(TextureSpecularEnabled);

MATERIAL_DEFINE_VARIABLE(Shininess);

class MaterialVariable
{
	ShaderVariable *m_ShaderVariable;

	// Cached values
	glm::mat4 m_Value;
	
public:
	MaterialVariable(ShaderVariable *var);

	const std::string &GetName() const;

	void SetBool(bool v);
	void SetInt(int v);
	void SetUInt(unsigned int v);
	void SetFloat(float v);
	void SetVec2(const glm::vec2 &v);
	void SetVec3(const glm::vec3 &v);
	void SetVec4(const glm::vec4 &v);
	void SetMat4(const glm::mat4 &v);

	bool GetBool() const;
	int GetInt() const;
	unsigned int GetUInt() const;
	float GetFloat() const;
	glm::vec2 GetVec2() const;
	glm::vec3 GetVec3() const;
	glm::vec4 GetVec4() const;
	glm::mat4 GetMat4() const;

	void Apply();
};

class MaterialResource
{
	std::string m_Name;
	Texture *m_Texture;
	unsigned int m_Slot; // Active texture slot/unit

public:
	MaterialResource(std::string name, Texture *texture, unsigned int slot);

	const std::string &GetName() const;
	Texture *GetTexture() const;
	unsigned int GetSlot() const;

	void SetTexture(Texture *texture);
};

class Material
{
	std::string m_Name;
	Shader *m_Shader;
	GraphicsManager *m_GraphicsManager;
	std::vector<MaterialVariable *> m_Variables;
	std::vector<MaterialResource *> m_Resources;

public:
	Material(std::string name, Shader *shader);
	~Material();

	// No copying/moving -- for now
	Material(const Material &) = delete;
	Material &operator=(const Material &) = delete;

	Material(const Material &&) = delete;
	Material &operator=(const Material &&) = delete;

	Texture *GetTexture(const std::string &name) const;
	unsigned int GetTextureSlot(const std::string &name) const;
	unsigned int SetTexture(const std::string &name, Texture *texture); // Not thread safe

	const std::string &GetName() const;

	Shader *GetShader() const;

	MaterialVariable *GetVariable(const std::string &name);
	std::vector<MaterialVariable *> GetVariables() const;

	void Apply();
};