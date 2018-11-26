#include "Skybox.h"

Skybox::Skybox(std::string name, Model *model, float scale)
	: Object(name), m_Model(model), m_Scale(scale)
{
	// Set position
	const auto transform = m_Model->GetTransform();
	transform->SetPosition(glm::vec3(0.0f));
	transform->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(90.0f));
	transform->SetScale(glm::vec3(scale));
}

void Skybox::Update(float time, float deltaTime)
{
}

void Skybox::Render(float time, float deltaTime)
{
}

void CreateSkybox(float scale, Mesh *mesh, Material *material, Object *parentObj, Node *parentNode)
{
	// Create model
	std::vector<Mesh *> meshes;
	meshes.push_back(mesh);
	std::vector<Material *> materials;
	materials.push_back(material);
	const auto model = parentNode->CreateChild<Model>("Skybox", meshes, materials, false); // Unmanaged memory

	// Create skybox
	parentObj->CreateChild<Skybox>("Skybox", model, scale);
}
