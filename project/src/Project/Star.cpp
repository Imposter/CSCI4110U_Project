#include "Star.h"
#include "Util.h"
#include "UVSphere.h"

Star::Star(std::string name, Model *model, const glm::vec3 &position, float minSize, float maxSize, float scaleRate)
	: Object(std::move(name)), m_Model(model), m_Position(position), m_MinSize(minSize), m_MaxSize(maxSize), m_ScaleRate(scaleRate)
{
}

void Star::Update(float time, float deltaTime)
{
	// Set position
	const auto transform = m_Model->GetTransform();
	transform->SetPosition(m_Position);

	// Make star "twinkle"
	const glm::vec3 size(m_MinSize + sin(time * m_ScaleRate) * m_MaxSize);
	transform->SetScale(size);
}

void Star::Render(float time, float deltaTime)
{
}

void GenerateStars(int count, float innerRadius, float outerRadius, float minSize, float maxSize, Shader *matShader, Object *parentObj, Node *parentNode)
{
	// Generate mesh
	const UVSphere sphere(STAR_RESOLUTION, STAR_RESOLUTION, 1.0f);

	// Generate spheres of random sizes
	for (auto i = 0; i < count; i++)
	{
		// Create material
		const auto material = New<Material>("StarMaterial", matShader);

		// Add randomization to color
		const auto brightness = RandomFloat(0.5f, 0.8f);

		const glm::vec3 color(1.0f * brightness, 1.0f * brightness, 1.0f * brightness);

		// Set material vars
		material->GetVariable(kMaterialVar_Diffuse)->SetVec3(color);

		// Get indices
		const auto &indices = sphere.GetIndices();

		// Store vertices
		std::vector<MeshVertex> vertices;
		const auto &positions = sphere.GetPositions();
		const auto &normals = sphere.GetNormals();
		const auto &texCoords = sphere.GetTextureCoords();
		for (size_t j = 0; j < positions.size(); j++)
			vertices.emplace_back(positions[j], normals[j], texCoords[j]);

		// Create mesh
		const auto mesh = New<Mesh>("Star", vertices, indices, material);

		// Create model
		std::vector<Mesh *> meshes;
		std::vector<Material *> materials;
		meshes.push_back(mesh);
		materials.push_back(material);
		const auto model = parentNode->CreateChild<Model>("Star", meshes, materials);

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

		// Create star
		parentObj->CreateChild<Star>(String::Format("Star%3d", i), model, pos, minSize, maxSize, scaleRate);
	}
}
