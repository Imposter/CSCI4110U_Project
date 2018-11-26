#pragma once

#include "Object.h"
#include "Shader.h"
#include "GraphicsManager.h"
#include "Utility/Exception.h"
#include <glm/glm.hpp>

#ifndef LIGHTS_MAX
#define LIGHTS_MAX 255
#endif

DEFINE_EXCEPTION(LightNotFoundException);

#define LIGHT_DEFINE_VARIABLE(name) static const char *kLightVar_ ## name =  "u_" #name
#define LIGHT_GET_BLOCK_VARIABLE(var, i, name) String::Format("%s[%d].%s", var, i, name)

// Vars
LIGHT_DEFINE_VARIABLE(ViewPosition);
LIGHT_DEFINE_VARIABLE(DirectionalLightCount);
LIGHT_DEFINE_VARIABLE(DirectionalLights);
LIGHT_DEFINE_VARIABLE(PointLightCount);
LIGHT_DEFINE_VARIABLE(PointLights);

enum LightType
{
	kLightType_Directional,
	kLightType_Point
};

class ILight : public Object
{
protected:
	unsigned int m_Type;

	// IsActive is used for enabled
	float m_Intensity;

	// Colors
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

public:
	ILight(std::string name, unsigned int type, Object *parent = nullptr);

	// No copying/moving
	ILight(const ILight &) = delete;
	ILight &operator=(const ILight &) = delete;

	ILight(const ILight &&) = delete;
	ILight &operator=(const ILight &&) = delete;

	unsigned int GetType() const;

	float GetIntensity() const;
	void SetIntensity(const float &intensity);

	const glm::vec3 &GetAmbient() const;
	void SetAmbient(const glm::vec3 &ambient);

	const glm::vec3 &GetDiffuse() const;
	void SetDiffuse(const glm::vec3 &diffuse);

	const glm::vec3 &GetSpecular() const;
	void SetSpecular(const glm::vec3 &specular);

	virtual void SetCount(Shader *shader, unsigned int count) = 0;
	virtual void Apply(Shader *shader, unsigned int index = 0) = 0;
};

class DirectionalLight : public ILight
{
	glm::vec3 m_Direction;

public:
	DirectionalLight(Object *parent = nullptr);

	const glm::vec3 &GetDirection() const;
	void SetDirection(const glm::vec3 &direction);

	void SetCount(Shader *shader, unsigned int count) override;
	void Apply(Shader *shader, unsigned int index) override;
};

class PointLight : public ILight
{
	glm::vec3 m_Position;

public:
	PointLight(Object *parent = nullptr);

	const glm::vec3 &GetPosition() const;
	void SetPosition(const glm::vec3 &position);

	void SetCount(Shader *shader, unsigned int count) override;
	void Apply(Shader *shader, unsigned int index) override;
};

// Not thread safe
class LightManager
{
	std::vector<ILight *> m_Lights;
	
public:
	LightManager() = default;
	~LightManager();

	// No copying/moving
	LightManager(const LightManager &) = delete;
	LightManager &operator=(const LightManager &) = delete;

	LightManager(const LightManager &&) = delete;
	LightManager &operator=(const LightManager &&) = delete;

	void Apply(Shader *shader, const glm::vec3 &viewPosition);

	template<class TLight, typename... TArgs>
	TLight *CreateLight(TArgs... args)
	{
		// Create light
		const auto light = New<TLight, TArgs...>(args...);

		// Store light
		m_Lights.push_back(light);

		return light;
	}

	void RemoveLight(ILight *light)
	{
		// Check if light exists
		for (auto it = m_Lights.begin(); it != m_Lights.end();)
		{
			if (*it == light)
			{
				m_Lights.erase(it);

				// Free memory
				Delete(light);

				return;
			}

			++it;
		}

		THROW_EXCEPTION(LightNotFoundException, "Light does not exist");
	}

	// TODO/NOTE: GetLight functions?
};