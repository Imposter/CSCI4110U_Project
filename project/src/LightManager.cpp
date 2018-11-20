#include "LightManager.h"

ILight::ILight()
	: m_Direction(0.0f), m_Intensity(0.0f), m_Ambient(0.0f), m_Diffuse(0.0f), m_Specular(0.0f)
{
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

// GraphicsManager::GetActiveShader?

// Or take a shader? -- todo: find out
LightManager::LightManager(GraphicsManager *graphicsManager)
	: m_GraphicsManager(graphicsManager)
{
	// TODO: Setup lights for shaders?
} // TODO: UniformBuffer class based on Buffer?

LightManager::~LightManager()
{
	for (const auto &l : m_Lights)
		Delete(l);
	m_Lights.clear();
}