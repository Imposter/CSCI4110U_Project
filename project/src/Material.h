#pragma once

#include "Texture.h"
#include "Shader.h"

DEFINE_EXCEPTION(MaterialVariableNotFoundException);
DEFINE_EXCEPTION(MaterialTextureNotFoundException);

class MaterialVariable
{
	ShaderVariable *m_ShaderVariable;

	// Cached values
	float m_FloatValue1;
	float m_FloatValue2;
	float m_FloatValue3;
	float m_FloatValue4;
	float m_MatrixValue4x4[4][4];
	
public:
	MaterialVariable(ShaderVariable *var);

	const std::string &GetName() const;

	void SetFloat1(float v1);
	void SetFloat2(float v1, float v2);
	void SetFloat3(float v1, float v2, float v3);
	void SetFloat4(float v1, float v2, float v3, float v4);
	void SetFloatMat4x4(float m1[4][4]);

	void GetFloat1(float &v1) const;
	void GetFloat2(float &v1, float &v2) const;
	void GetFloat3(float &v1, float &v2, float &v3) const;
	void GetFloat4(float &v1, float &v2, float &v3, float &v4) const;
	void GetFloatMat4x4(float m1[4][4]) const;
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
	Shader *m_Shader;
	std::vector<MaterialVariable *> m_Variables;
	std::vector<MaterialResource *> m_Resources;

	void init();

public:
	Material(Shader *shader);
	~Material();

	// No copying/moving -- for now
	Material(const Material &) = delete;
	Material &operator=(const Material &) = delete;

	Material(const Material &&) = delete;
	Material &operator=(const Material &&) = delete;

	Texture *GetTexture(const std::string &name) const;
	unsigned int GetTextureSlot(const std::string &name) const;
	void SetTexture(const std::string &name, Texture *texture); // Not thread safe

	Shader *GetShader() const;
	void SetShader(Shader *shader);

	MaterialVariable *GetVariable(const std::string &name) const;
	std::vector<MaterialVariable *> GetVariables() const;

	void Apply();
};

// TODO: MaterialDefinition