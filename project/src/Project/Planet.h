#pragma once

#include "../Object.h"
#include "../Model.h"

#ifndef PLANET_RESOLUTION
#define PLANET_RESOLUTION 24
#endif

class Planet : public Object
{
	Model *m_Model;

	// TODO: ...
public:
	Planet(std::string name, Model *model, const glm::vec3 &position, float size);
};

void GeneratePlanet(const std::string &name, const glm::vec3 &position, float radius, float rotationSpeed, Texture *texture, Shader *matShader, 
	Object *parentObj, Node *parentNode);