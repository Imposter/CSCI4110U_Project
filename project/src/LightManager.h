#pragma once

#include "Object.h"
#include <glm/glm.hpp>
#include <vector>
#include "GraphicsManager.h"

#ifndef LIGHTS_MAX
#define LIGHTS_MAX 255
#endif

class ILight : public Object
{
protected:
	// IsActive is used for enabled
	glm::vec3 m_Direction;
	float m_Intensity;

	// Colors
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

public:
	ILight();

	// No copying/moving
	ILight(const ILight &) = delete;
	ILight &operator=(const ILight &) = delete;

	ILight(const ILight &&) = delete;
	ILight &operator=(const ILight &&) = delete;

	// ...
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
};

// TODO: Define block types (however we cannot abstract or we will run into issues with alignment
// TODO: Therefore we must split lights into directional, points, spots

class LightManager
{
	GraphicsManager *m_GraphicsManager; // We might not even need this
	std::vector<ILight *> m_Lights;
	
public:
	// ...
	LightManager(GraphicsManager *graphicsManager);
	~LightManager();

	// No copying/moving
	LightManager(const LightManager &) = delete;
	LightManager &operator=(const LightManager &) = delete;

	LightManager(const LightManager &&) = delete;
	LightManager &operator=(const LightManager &&) = delete;

	// TODO: Update();
	// TODO: ApplyTo(Shader *);

	// TODO/NOTE: We shouldn't need this
	template<class TLight, typename... TArgs>
	TLight *CreateLight(TArgs... args)
	{
		const auto light = New<TLight, TArgs...>(args...);
		m_Lights.push_back(light);
		return light;
	}
};