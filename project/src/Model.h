#pragma once

#include "Node.h"
#include "Mesh.h"
#include "Utility/Exception.h"

DEFINE_EXCEPTION(MeshNotFoundException);
DEFINE_EXCEPTION(MaterialNotFoundException);

class Model : public Node
{
	std::vector<Mesh *> m_Meshes; // TODO: Move this to Model::m_Children later
	std::vector<Material *> m_Materials;
	bool m_Managed;

public:
	Model(std::string name, std::vector<Mesh *> meshes, std::vector<Material *> materials, bool managed = true);
	~Model();

	// No copying/moving
	Model(const Model &) = delete;
	Model &operator=(const Model &) = delete;

	Model(const Model &&) = delete;
	Model &operator=(const Model &&) = delete;

	// TODO: Create new managed material? -- load all possible materials?

	Mesh *GetMesh(const std::string &name) const;
	Material *GetMaterial(const std::string &name) const;

	void Compile() override;
	void Render(RenderContext *context) override;
};