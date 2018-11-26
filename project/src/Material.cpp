#include "Material.h"
#include "Memory.h"

MaterialVariable::MaterialVariable(ShaderVariable *var)
	: m_ShaderVariable(var), m_Value(0.0f)
{
}

const std::string &MaterialVariable::GetName() const
{
	return m_ShaderVariable->GetName();
}

// TODO: Move caching to ShaderVariable
void MaterialVariable::SetBool(bool v)
{
	m_Value[0].x = static_cast<float>(v);
	m_ShaderVariable->SetBool(v);
}

void MaterialVariable::SetInt(int v)
{
	m_Value[0].x = static_cast<float>(v);
	m_ShaderVariable->SetInt(v);
}

void MaterialVariable::SetUInt(unsigned int v)
{
	m_Value[0].x = static_cast<float>(v);
	m_ShaderVariable->SetUInt(v);
}

void MaterialVariable::SetFloat(float v)
{
	m_Value[0].x = v;
	m_ShaderVariable->SetFloat(v);
}

void MaterialVariable::SetVec2(const glm::vec2 &v)
{
	m_Value[0].x = v.x;
	m_Value[0].y = v.y;
	m_ShaderVariable->SetVec2(v);
}

void MaterialVariable::SetVec3(const glm::vec3 &v)
{
	m_Value[0].x = v.x;
	m_Value[0].y = v.y;
	m_Value[0].z = v.z;
	m_ShaderVariable->SetVec3(v);
}

void MaterialVariable::SetVec4(const glm::vec4 &v)
{
	m_Value[0].x = v.x;
	m_Value[0].y = v.y;
	m_Value[0].z = v.z;
	m_Value[0].w = v.w;
	m_ShaderVariable->SetVec4(v);
}

void MaterialVariable::SetMat4(const glm::mat4 &v)
{
	m_Value = v;
	m_ShaderVariable->SetMat4(v);
}

bool MaterialVariable::GetBool() const
{
	return static_cast<bool>(m_Value[0].x);
}

int MaterialVariable::GetInt() const
{
	return static_cast<int>(m_Value[0].x);
}

unsigned int MaterialVariable::GetUInt() const
{
	return static_cast<unsigned int>(m_Value[0].x);
}

float MaterialVariable::GetFloat() const
{
	return m_Value[0].x;
}

glm::vec2 MaterialVariable::GetVec2() const
{
	return { m_Value[0].x, m_Value[0].y };
}

glm::vec3 MaterialVariable::GetVec3() const
{
	return { m_Value[0].x, m_Value[0].y, m_Value[0].z };
}

glm::vec4 MaterialVariable::GetVec4() const
{
	return { m_Value[0].x, m_Value[0].y, m_Value[0].z, m_Value[0].w };
}

glm::mat4 MaterialVariable::GetMat4() const
{
	return m_Value;
}

void MaterialVariable::Apply()
{
	// Disable variable type checking
	m_ShaderVariable->SetTypeCheck(false);

	switch (m_ShaderVariable->GetType())
	{
	case kShaderVariableType_Bool:
		m_ShaderVariable->SetBool(static_cast<bool>(m_Value[0].x));
		break;
	case kShaderVariableType_Int:
	case kShaderVariableType_Sampler2D:
		m_ShaderVariable->SetInt(static_cast<int>(m_Value[0].x));
		break;
	case kShaderVariableType_UInt:
		m_ShaderVariable->SetUInt(static_cast<unsigned int>(m_Value[0].x));
		break;
	case kShaderVariableType_Float:
		m_ShaderVariable->SetFloat(m_Value[0].x);
		break;
	case kShaderVariableType_Vec2:
		m_ShaderVariable->SetVec2({ m_Value[0].x, m_Value[0].y });
		break;
	case kShaderVariableType_Vec3:
		m_ShaderVariable->SetVec3({ m_Value[0].x, m_Value[0].y, m_Value[0].z });
		break;
	case kShaderVariableType_Vec4:
		m_ShaderVariable->SetVec4(m_Value[0]);
		break;
	case kShaderVariableType_Mat4:
		m_ShaderVariable->SetMat4(m_Value);
		break;
	default:
		// Enable variable type checking
		m_ShaderVariable->SetTypeCheck(true);

		THROW_EXCEPTION(MaterialUnsupportedTypeException, "Unknown variable type");
	}

	// Enable variable type checking
	m_ShaderVariable->SetTypeCheck(true);
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

Material::Material(std::string name, Shader *shader)
	: m_Name(std::move(name)), m_Shader(shader)
{
	// Read shader vars and make material vars for them
	for (auto &var : m_Shader->GetVariables())
	{
		// Only store variables if they are referring to the material
		if (var->GetName().substr(0, strlen(MATERIAL_KEY_NAME)) == MATERIAL_KEY_NAME)
			m_Variables.push_back(New<MaterialVariable>(var));
	}
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
unsigned int Material::SetTexture(const std::string &name, Texture *texture) // TODO: Test
{
	// Check if texture with name is already present
	for (auto &res : m_Resources) // TODO: This should take parameters without "u_Material.*" in them and correct them...
	{
		if (res->GetName() == name)
		{
			res->SetTexture(texture);
			return res->GetSlot();
		}
	}

	// Add resource
	const auto slot = m_Resources.size();
	m_Resources.push_back(New<MaterialResource>(name, texture, slot));
	return slot;
}

const std::string & Material::GetName() const
{
	return m_Name;
}

Shader *Material::GetShader() const
{
	return m_Shader;
}

bool Material::IsVariable(const std::string &name) const
{
	for (auto &var : m_Variables)
	{
		if (var->GetName() == name)
			return true;
	}

	return false;
}

MaterialVariable *Material::GetVariable(const std::string &name)
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
	// Activate shader
	m_Shader->Use();

	// Apply material vars
	for (auto &var : m_Variables)
		var->Apply();

	// Apply textures to texture units
	for (auto &res : m_Resources)
	{
		const auto texture = res->GetTexture();
		const auto slot = res->GetSlot();
		const auto var = m_Shader->GetVariable(res->GetName());

		texture->Activate(slot);

		// Set slot
		var->SetTypeCheck(false);
		var->SetInt(slot);
		var->SetTypeCheck(true);
	}
}
