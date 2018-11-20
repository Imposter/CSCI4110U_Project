#include "LightManager.h"
#include <map>

ILight::ILight(std::string name, unsigned int type, Object *parent)
	: Object(name, parent), m_Type(type), m_Direction(0.0f), m_Intensity(0.0f), m_Ambient(0.0f), m_Diffuse(0.0f), m_Specular(0.0f)
{
}

unsigned int ILight::GetType() const
{
	return m_Type;
}

const glm::vec3 &ILight::GetDirection() const
{
	return m_Direction;
}

void ILight::SetDirection(const glm::vec3 &direction)
{
	m_Direction = direction;
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

const glm::vec3 &ILight::GetSpecular() const
{
	return m_Specular;
}

void ILight::SetSpecular(const glm::vec3 &specular)
{
	m_Specular = specular;
}

LightManager::~LightManager()
{
	for (const auto &l : m_Lights)
		Delete(l);
	m_Lights.clear();
}

void LightManager::Apply(Shader *shader)
{
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
		shader->GetVariable(String::Format("u_LightCount[%u]", p.first))->SetUInt(p.second); // TODO: Can we make this neater any way?
}
