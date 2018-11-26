#include "LightManager.h"
#include <map>

ILight::ILight(std::string name, unsigned int type, Object *parent)
	: Object(std::move(name), parent), m_Type(type), m_Intensity(0.0f), m_Ambient(0.0f), m_Diffuse(0.0f), m_Specular(0.0f)
{
}

unsigned int ILight::GetType() const
{
	return m_Type;
}

float ILight::GetIntensity() const
{
	return m_Intensity;
}

void ILight::SetIntensity(const float &intensity)
{
	m_Intensity = intensity;
}

const glm::vec3 &ILight::GetAmbient() const
{
	return m_Ambient;
}

void ILight::SetAmbient(const glm::vec3 &ambient)
{
	m_Ambient = ambient;
}

const glm::vec3 &ILight::GetDiffuse() const
{
	return m_Diffuse;
}

void ILight::SetDiffuse(const glm::vec3 &diffuse)
{
	m_Diffuse = diffuse;
}

const glm::vec3 & ILight::GetSpecular() const
{
	return m_Specular;
}

void ILight::SetSpecular(const glm::vec3 &specular)
{
	m_Specular = specular;
}

DirectionalLight::DirectionalLight(Object *parent)
	: ILight("DirectionalLight", kLightType_Directional, parent), m_Direction(0.0f)
{
}

const glm::vec3 &DirectionalLight::GetDirection() const
{
	return m_Direction;
}

void DirectionalLight::SetDirection(const glm::vec3 &direction)
{
	m_Direction = direction;
}

void DirectionalLight::SetCount(Shader * shader, unsigned int count)
{
	shader->GetVariable(kLightVar_DirectionalLightCount)->SetInt(count);
}

void DirectionalLight::Apply(Shader *shader, unsigned int index)
{
	shader->GetVariable(LIGHT_GET_BLOCK_VARIABLE(kLightVar_DirectionalLights, index, "Direction"))->SetVec3(m_Direction);
	shader->GetVariable(LIGHT_GET_BLOCK_VARIABLE(kLightVar_DirectionalLights, index, "Intensity"))->SetFloat(m_Intensity);

	shader->GetVariable(LIGHT_GET_BLOCK_VARIABLE(kLightVar_DirectionalLights, index, "Ambient"))->SetVec3(m_Ambient);
	shader->GetVariable(LIGHT_GET_BLOCK_VARIABLE(kLightVar_DirectionalLights, index, "Diffuse"))->SetVec3(m_Diffuse);
	shader->GetVariable(LIGHT_GET_BLOCK_VARIABLE(kLightVar_DirectionalLights, index, "Specular"))->SetVec3(m_Specular);
}

PointLight::PointLight(Object *parent)
	: ILight("PointLight", kLightType_Point, parent), m_Position(0.0f)
{
}

const glm::vec3 &PointLight::GetPosition() const
{
	return m_Position;
}

void PointLight::SetPosition(const glm::vec3 &position)
{
	m_Position = position;
}

void PointLight::SetCount(Shader * shader, unsigned int count)
{
	shader->GetVariable(kLightVar_PointLightCount)->SetInt(count);
}

void PointLight::Apply(Shader *shader, unsigned int index)
{
	shader->GetVariable(LIGHT_GET_BLOCK_VARIABLE(kLightVar_PointLights, index, "Position"))->SetVec3(m_Position);
	shader->GetVariable(LIGHT_GET_BLOCK_VARIABLE(kLightVar_PointLights, index, "Intensity"))->SetFloat(m_Intensity);

	shader->GetVariable(LIGHT_GET_BLOCK_VARIABLE(kLightVar_PointLights, index, "Ambient"))->SetVec3(m_Ambient);
	shader->GetVariable(LIGHT_GET_BLOCK_VARIABLE(kLightVar_PointLights, index, "Diffuse"))->SetVec3(m_Diffuse);
	shader->GetVariable(LIGHT_GET_BLOCK_VARIABLE(kLightVar_PointLights, index, "Specular"))->SetVec3(m_Specular);
}

LightManager::~LightManager()
{
	for (const auto &l : m_Lights)
		Delete(l);
	m_Lights.clear();
}

void LightManager::Apply(Shader *shader, const glm::vec3 &viewPosition)
{
	// Use shader
	shader->Use();

	// Set view position
	shader->GetVariable(kLightVar_ViewPosition)->SetVec3(viewPosition);

	std::map<unsigned int, unsigned int> typeCount;
	
	for (const auto &l : m_Lights)
	{
		// Ignore inactive lights
		if (!l->IsActive())
			continue;

		// Increase index of active light
		const auto idx = typeCount[l->GetType()];
		++typeCount[l->GetType()];

		// Apply light to shader
		l->Apply(shader, idx);
	}

	// Store counts in shader
	for (const auto &p : typeCount)
	{
		for (const auto &l : m_Lights)
		{
			if (l->GetType() == p.first)
			{
				l->SetCount(shader, p.second);
				break;
			}
		}
	}
}