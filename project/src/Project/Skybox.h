#pragma once

#include "../Object.h"
#include "../Model.h"

class Skybox : public Object // TODO: Delete class
{
	Model *m_Model;
	float m_Scale;

public:
	Skybox(std::string name, Model *model, float scale);

	// No copying/moving
	Skybox(const Skybox &) = delete;
	Skybox &operator=(const Skybox &) = delete;

	Skybox(const Skybox &&) = delete;
	Skybox &operator=(const Skybox &&) = delete;

	void Update(float time, float deltaTime) override;
	void Render(float time, float deltaTime) override;
};

void CreateSkybox(float scale, Mesh *mesh, Material *material, Object *parentObj, Node *parentNode);