#pragma once

#include "Mesh.h"
#include "Utility/Exception.h"

DEFINE_EXCEPTION(MeshNotFoundException);
DEFINE_EXCEPTION(MaterialNotFoundException);

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

	// TODO: Create new managed material? -- load all possible materials?

	Mesh *GetMesh(const std::string &name) const;
	Material *GetMaterial(const std::string &name) const;

	void Compile();
	void Render(RenderContext *context);
};