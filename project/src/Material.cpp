#include "Material.h"
#include "Memory.h"

MaterialVariable::MaterialVariable(ShaderVariable *var)
	: m_ShaderVariable(var), m_FloatValue1(0.0f), m_FloatValue2(0.0f), m_FloatValue3(0.0f), 
	m_FloatValue4(0.0f), m_MatrixValue4x4{0.0f}
{
}

const std::string &MaterialVariable::GetName() const
{
	return m_ShaderVariable->GetName();
}

void MaterialVariable::SetFloat1(float v1)
{
	m_FloatValue1 = v1;
	m_ShaderVariable->SetFloat1(v1);
}

void MaterialVariable::SetFloat2(float v1, float v2)
{
	m_FloatValue1 = v1;
	m_FloatValue2 = v2;
	m_ShaderVariable->SetFloat2(v1, v2);
}

void MaterialVariable::SetFloat3(float v1, float v2, float v3)
{
	m_FloatValue1 = v1;
	m_FloatValue2 = v2;
	m_FloatValue3 = v3;
	m_ShaderVariable->SetFloat3(v1, v2, v3);
}

void MaterialVariable::SetFloat4(float v1, float v2, float v3, float v4)
{
	m_FloatValue1 = v1;
	m_FloatValue2 = v2;
	m_FloatValue3 = v3;
	m_FloatValue4 = v4;
	m_ShaderVariable->SetFloat4(v1, v2, v3, v4);
}

void MaterialVariable::SetFloatMat4x4(float m1[4][4])
{
	memcpy(m_MatrixValue4x4, m1, sizeof(m_MatrixValue4x4)); // TODO: Test
	m_ShaderVariable->SetVecMatrixFloat4(1, false, reinterpret_cast<float *>(m1));
}

void MaterialVariable::GetFloat1(float &v1) const
{
	v1 = m_FloatValue1;
}

void MaterialVariable::GetFloat2(float &v1, float &v2) const
{
	v1 = m_FloatValue1;
	v2 = m_FloatValue2;
}

void MaterialVariable::GetFloat3(float &v1, float &v2, float &v3) const
{
	v1 = m_FloatValue1;
	v2 = m_FloatValue2;
	v3 = m_FloatValue3;
}

void MaterialVariable::GetFloat4(float &v1, float &v2, float &v3, float &v4) const
{
	v1 = m_FloatValue1;
	v2 = m_FloatValue2;
	v3 = m_FloatValue3;
	v4 = m_FloatValue4;
}

void MaterialVariable::GetFloatMat4x4(float m1[4][4]) const
{
	memcpy(m1, m_MatrixValue4x4, sizeof(m_MatrixValue4x4)); // TODO: Test
}

MaterialResource::MaterialResource(std::string name, Texture *texture, unsigned int slot)
	: m_Name(std::move(name)), m_Texture(texture), m_Slot(slot)
{
}

const std::string &MaterialResource::GetName() const
{
	return m_Name;
}

Texture *MaterialResource::GetTexture() const
{
	return m_Texture;
}

unsigned MaterialResource::GetSlot() const
{
	return m_Slot;
}

void MaterialResource::SetTexture(Texture *texture)
{
	m_Texture = texture;
}

void Material::init()
{
	// Remove existing vars
	for (auto &var : m_Variables)
		Delete(var);

	m_Variables.clear();

	// Read shader vars and make material vars for them
	for (auto &var : m_Shader->GetVariables())
		m_Variables.push_back(New<MaterialVariable>(var));
}

Material::Material(Shader *shader)
	: m_Shader(shader)
{
	init();
}

Material::~Material()
{
	for (auto &var : m_Variables)
		Delete(var);

	m_Variables.clear();

	for (auto &res : m_Resources)
		Delete(res);

	m_Resources.clear();
}

Texture *Material::GetTexture(const std::string &name) const
{
	for (auto &res : m_Resources)
	{
		if (res->GetName() == name)
			return res->GetTexture();
	}

	THROW_EXCEPTION(MaterialTextureNotFoundException, "Texture %s not found", name.c_str());
}

unsigned int Material::GetTextureSlot(const std::string &name) const
{
	for (auto &res : m_Resources)
	{
		if (res->GetName() == name)
			return res->GetSlot();
	}

	THROW_EXCEPTION(MaterialTextureNotFoundException, "Texture %s not found", name.c_str());
}

// Active texture limit is 16/32
void Material::SetTexture(const std::string &name, Texture *texture)
{
	// Check if texture with name is already present
	for (auto &res : m_Resources)
	{
		if (res->GetName() == name)
		{
			res->SetTexture(texture);
			return;
		}
	}

	// Add resource
	m_Resources.push_back(New<MaterialResource>(name, texture, m_Resources.size()));
}

Shader *Material::GetShader() const
{
	return m_Shader;
}

void Material::SetShader(Shader *shader)
{
	m_Shader = shader;

	init(); // init vars again
}

MaterialVariable *Material::GetVariable(const std::string &name) const
{
	for (auto &var : m_Variables)
	{
		if (var->GetName() == name)
			return var;
	}

	THROW_EXCEPTION(MaterialVariableNotFoundException, "Variable %s not found", name.c_str());
}

std::vector<MaterialVariable *> Material::GetVariables() const
{
	return m_Variables;
}

void Material::Apply()
{
	// Use shader
	m_Shader->Use();

	// Apply textures to texture units
	for (auto &res : m_Resources)
	{
		const auto texture = res->GetTexture();
		const auto slot = res->GetSlot();
		const auto var = m_Shader->GetVariable(res->GetName());

		texture->Activate(slot);
		var->SetUInt1(slot);
	}
}
