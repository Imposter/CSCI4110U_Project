#include "Star.h"
#include "Util.h"

Star::Star(std::string name, Model *model, const glm::vec3 &position, float minSize, float maxSize, float scaleRate)
	: Object(std::move(name)), m_Model(model), m_Position(position), m_MinSize(minSize), m_MaxSize(maxSize), m_ScaleRate(scaleRate)
{
	// Set position
	const auto transform = m_Model->GetTransform();
	transform->SetPosition(m_Position);
}

void Star::Update(float time, float deltaTime)
{
	const auto transform = m_Model->GetTransform();
	
	// Make star "twinkle"
	const glm::vec3 size(m_MinSize + sin(time / 1000.0f * m_ScaleRate) * m_MaxSize);
	transform->SetScale(size);
}

void Star::Render(float time, float deltaTime)
{
}

void CreateStarField(int count, float innerRadius, float outerRadius, float minSize, float maxSize, Mesh *mesh, Material *material, Object *parentObj, Node *parentNode)
{
	// Generate spheres of random sizes
	for (auto i = 0; i < count; i++)
	{
		// Create random position
		auto posX = RandomFloat(innerRadius, outerRadius);
		auto posY = RandomFloat(innerRadius, outerRadius);
		auto posZ = RandomFloat(innerRadius, outerRadius);

		// Negate positions
		if (RandomInt(0, 2) == 0) posX *= -1.0f;
		if (RandomInt(0, 2) == 0) posY *= -1.0f;
		if (RandomInt(0, 2) == 0) posZ *= -1.0f;

		const glm::vec3 pos(posX, posY, posZ);

		// Create random scale rate
		const auto scaleRate = RandomFloat(0.25, 0.75);

		// Create model
		std::vector<Mesh *> meshes;
		meshes.push_back(mesh);
		std::vector<Material *> materials;
		materials.push_back(material);
		const auto model = parentNode->CreateChild<Model>("Star", meshes, materials, false); // Unmanaged memory

		// Create star
		parentObj->CreateChild<Star>(String::Format("Star_%3d", i), model, pos, minSize, maxSize, scaleRate);
	}
}