#pragma once

#include "Object.h"
#include "Shader.h"
#include "Utility/Exception.h"
#include <glm/glm.hpp>
#include <vector>

#ifndef LIGHTS_MAX
#define LIGHTS_MAX 255
#endif

DEFINE_EXCEPTION(LightNotFoundException);

// NOTE: This class is unused... for now.

// TODO: LightDefineVar? -- kind of like in Material?

// TODO: GetVar function, typeName so we can just Shader->GetVar("u_Lights[type][index].prop")
class ILight : public Object
{
protected:
	unsigned int m_Type;

	// IsActive is used for enabled
	glm::vec3 m_Direction;
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

	const glm::vec3 &GetDirection() const;
	void SetDirection(const glm::vec3 &direction);

	float GetIntensity() const;
	void SetIntensity(const float &intensity);

	const glm::vec3 &GetAmbient() const;
	void SetAmbient(const glm::vec3 &ambient);

	const glm::vec3 &GetDiffuse() const;
	void SetDiffuse(const glm::vec3 &diffuse);

	const glm::vec3 &GetSpecular() const;
	void SetSpecular(const glm::vec3 &specular);

	virtual void Apply(Shader *shader, unsigned int index) = 0;
};

// Not thread safe
class LightManager
{
	std::vector<ILight *> m_Lights;
	
public:
	~LightManager();

	// No copying/moving
	LightManager(const LightManager &) = delete;
	LightManager &operator=(const LightManager &) = delete;

	LightManager(const LightManager &&) = delete;
	LightManager &operator=(const LightManager &&) = delete;

	void Apply(Shader *shader);

	template<class TLight, typename... TArgs>
	TLight *CreateLight(TArgs... args)
	{
		// Create light
		ILight *light = New<TLight, TArgs...>(args...);

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