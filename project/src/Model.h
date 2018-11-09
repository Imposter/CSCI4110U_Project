#pragma once

#include "Mesh.h"

class Model
{
	std::string m_Name;
	std::vector<Mesh *> m_Meshes;
	std::vector<Material *> m_Materials;

public:
	Model(std::string name, std::vector<Mesh *> meshes, std::vector<Material *> materials);
	~Model();

	// No copying/moving
	Model(const Model &) = delete;
	Model &operator=(const Model &) = delete;

	Model(const Model &&) = delete;
	Model &operator=(const Model &&) = delete;

	void Compile();
	void Render(RenderContext *context);
};