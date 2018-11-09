#include "Model.h"

Model::Model(std::string name, std::vector<Mesh *> meshes, std::vector<Material *> materials)
	: m_Name(std::move(name)), m_Meshes(std::move(meshes)), m_Materials(std::move(materials))
{
}

Model::~Model()
{
	// Delete meshes
	for (auto &m : m_Meshes)
		Delete(m);
	m_Meshes.clear();

	// Delete materials
	for (auto &m : m_Materials)
		Delete(m);
	m_Materials.clear();
}

void Model::Compile()
{
	for (auto &m : m_Meshes)
		m->Compile();
}

void Model::Render(RenderContext *context)
{
	for (auto &m : m_Meshes)
		m->Render(context);
}
