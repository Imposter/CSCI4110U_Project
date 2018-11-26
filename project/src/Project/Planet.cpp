#include "Planet.h"
#include "UVSphere.h"

/*void GeneratePlanet(const std::string &name, const glm::vec3 &position, float radius, float rotationSpeed,
	Texture *texture, Shader *matShader, Object *parentObj, Node *parentNode)
{
	// Generate mesh
	const UVSphere sphere(PLANET_RESOLUTION, PLANET_RESOLUTION, 1.0f);

	// Create material
	const auto material = New<Material>("PlanetMaterial", matShader);

	// Set material vars
	material->SetTexture(kMaterialVar_TextureDiffuse, texture);
	material->GetVariable(kMaterialVar_TextureDiffuseEnabled)->SetBool(true);

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
	const auto mesh = New<Mesh>("Planet", vertices, indices, material);

	// Create model
	std::vector<Mesh *> meshes;
	std::vector<Material *> materials;
	meshes.push_back(mesh);
	materials.push_back(material);
	const auto model = parentNode->CreateChild<Model>("Planet", meshes, materials);

	// Create star
	parentObj->CreateChild<Planet>(String::Format("Star_%s", name.c_str()), model, pos, minSize, maxSize, scaleRate);
}*/