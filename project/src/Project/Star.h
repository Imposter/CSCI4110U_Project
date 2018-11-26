#pragma once

#include "../Object.h"
#include "../Model.h"

#ifndef STAR_RESOLUTION 
#define STAR_RESOLUTION 12
#endif

class Star : public Object
{
	Model *m_Model;
	glm::vec3 m_Position;
	float m_MinSize;
	float m_MaxSize;
	float m_ScaleRate;

public:
	Star(std::string name, Model *model, const glm::vec3 &position, float minSize, float maxSize, float scaleRate);
	
	// No copying/moving
	Star(const Star &) = delete;
	Star &operator=(const Star &) = delete;

	Star(const Star &&) = delete;
	Star &operator=(const Star &&) = delete;

	void Update(float time, float deltaTime) override;
	void Render(float time, float deltaTime) override;
};

void CreateStarField(int count, float innerRadius, float outerRadius, float minSize, float maxSize, Mesh *mesh, Material *material, Object *parentObj, Node *parentNode);