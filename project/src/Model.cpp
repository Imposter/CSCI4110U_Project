#include "Model.h"
#include "Camera.h" 
// TODO: Abstract camera

Model::Model(std::string name, std::vector<Mesh *> meshes, std::vector<Material *> materials, bool managed)
	: Node(std::move(name)), m_Meshes(std::move(meshes)), m_Materials(std::move(materials)), m_Managed(managed)
{
}

Model::~Model()
{
	if (m_Managed) 
	{
		// Delete meshes
		for (auto &m : m_Meshes)
			Delete(m);

		// Delete materials
		for (auto &m : m_Materials)
			Delete(m);
	}

	m_Meshes.clear();
	m_Materials.clear();
}

Mesh *Model::GetMesh(const std::string &name) const
{
	for (auto &m : m_Meshes)
	{
		if (m->GetName() == name)
			return m;
	}

	THROW_EXCEPTION(MeshNotFoundException, "Mesh %s not found", name.c_str());
}

Material *Model::GetMaterial(const std::string &name) const
{
	for (auto &m : m_Materials)
	{
		if (m->GetName() == name)
			return m;
	}

	THROW_EXCEPTION(MaterialNotFoundException, "Material %s not found", name.c_str());
}

void Model::Compile()
{
	for (auto &m : m_Meshes)
		m->Compile();

	Node::Compile();
}

void Model::Render(RenderContext *context)
{
	// Check if we're in the bounding frustum of the camera
	// TODO: Causing issues with camera look at
	//const auto frustum = context->Camera->GetBoundingFrustum();
	//if (frustum.Contains(m_Transform.GetPosition()) != kContainmentType_Disjoint)
	{
		// Set transform matrix
		context->TransformMatrix = m_Transform.GetMatrix();

		for (auto &m : m_Meshes)
			m->Render(context);
	}

	Node::Render(context);
}
