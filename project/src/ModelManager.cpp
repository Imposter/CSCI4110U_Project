#include "ModelManager.h"
#include "Utility/FileUtil.h"
#include <assimp/postprocess.h>
#include <rapidjson/document.h>

void ModelManager::loadTexture(Material *material, const std::string &path, const std::string &key, const std::string &enableKey)
{
	// Load texture
	auto name = path.substr(path.find_last_of('/') + 1);
	name = name.substr(0, name.find_last_of('.'));
	const auto texture = m_GraphicsManager->GetTexture(name);

	// Set material texture
	material->SetTexture(key, texture);
	
	// Set material to use texture
	if (!enableKey.empty())
	{
		const auto v = material->GetVariable(enableKey);
		v->SetBool(true);
	}
}

Mesh *ModelManager::processMesh(Material *material, aiMesh *mesh, const aiScene *scene)
{
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;

	// Build vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex v;

		// Position
		v.Position.x = mesh->mVertices[i].x;
		v.Position.y = mesh->mVertices[i].y;
		v.Position.z = mesh->mVertices[i].z;

		// Normal
		if (mesh->HasNormals())
		{
			v.Normal.x = mesh->mNormals[i].x;
			v.Normal.y = mesh->mNormals[i].y;
			v.Normal.z = mesh->mNormals[i].z;
		}

		// Texture coordinates
		if (mesh->HasTextureCoords(0))
		{
			v.TexCoords.x = mesh->mTextureCoords[0][i].x;
			v.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(v);
	}

	// Build indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const auto face = mesh->mFaces[i];

		// Store indices of face
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return New<Mesh>(mesh->mName.C_Str(), vertices, indices, material);
}

void ModelManager::processNode(std::vector<Material *> &materials, std::vector<Mesh *> &meshes, aiNode *node,
	const aiScene *scene)
{
	// Load meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		const auto mesh = scene->mMeshes[node->mMeshes[i]];

		// Get material for mesh
		aiString reqMatName;
		if (scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_NAME, reqMatName) != AI_SUCCESS)
			THROW_EXCEPTION(ModelLoadException, "Unable to get material name");

		Material *material = nullptr;
		for (auto &m : materials)
		{
			if (m->GetName() == reqMatName.C_Str())
			{
				material = m;
				break;
			}
		}

		// Process mesh
		meshes.push_back(processMesh(material, mesh, scene));
	}

	// Load child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		const auto n = node->mChildren[i];
		processNode(materials, meshes, n, scene);
	}
}

Material *ModelManager::processMaterial(std::map<std::string, std::string> &materialMap, aiMaterial *material, const aiScene *scene)
{
	// Read material info
	aiString name;
	if (material->Get(AI_MATKEY_NAME, name) != AI_SUCCESS)
		THROW_EXCEPTION(ModelLoadException, "Unable to get material name, possibly no materials");

	// Check if material map contains shader for this material
	std::map<std::string, std::string>::iterator it;
	if ((it = materialMap.find(name.C_Str())) == materialMap.end())
		THROW_EXCEPTION(ModelLoadException, "Unable to get shader name, material must be mapped to a shader");

	// Find and activate shader
	const auto s = m_GraphicsManager->GetShader(it->second);
	s->Use();

	// Create material
	const auto m = New<Material>(name.C_Str(), s);

	// Colors
	{
		aiColor3D value;
		if (material->Get(AI_MATKEY_COLOR_AMBIENT, value) == AI_SUCCESS)
		{
			const auto v = m->GetVariable(kMaterialVar_Ambient);
			v->SetVec3({ value.r, value.g, value.b });
		}
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, value) == AI_SUCCESS)
		{
			const auto v = m->GetVariable(kMaterialVar_Diffuse);
			v->SetVec3({ value.r, value.g, value.b });
		}
		if (material->Get(AI_MATKEY_COLOR_SPECULAR, value) == AI_SUCCESS)
		{
			const auto v = m->GetVariable(kMaterialVar_Specular);
			v->SetVec3({ value.r, value.g, value.b });
		}
	}

	// Textures (NOTE: only one per property is supported)
	{
		aiString path;
		if (material->GetTextureCount(aiTextureType_AMBIENT))
			if (material->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS)
				loadTexture(m, path.C_Str(), kMaterialVar_TextureAmbient, kMaterialVar_TextureAmbientEnabled);
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
				loadTexture(m, path.C_Str(), kMaterialVar_TextureDiffuse, kMaterialVar_TextureDiffuseEnabled);
		if (material->GetTextureCount(aiTextureType_SPECULAR))
			if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
				loadTexture(m, path.C_Str(), kMaterialVar_TextureSpecular, kMaterialVar_TextureSpecularEnabled);
	}

	// Other properties
	{
		float value;
		if (material->Get(AI_MATKEY_SHININESS, value) == AI_SUCCESS)
		{
			const auto v = m->GetVariable(kMaterialVar_Shininess);
			v->SetFloat(value);
		}
	}

	// TODO: etc...

	return m;
}

void ModelManager::processScene(const aiScene *scene, std::map<std::string, std::string> &materialMap, std::vector<Material *> &materials, std::vector<Mesh *> &meshes)
{
	// Load materials
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		const auto m = scene->mMaterials[i];
		materials.push_back(processMaterial(materialMap, m, scene));
	}

	// Process nodes
	processNode(materials, meshes, scene->mRootNode, scene);
}

// TODO: Preferrably rewrite loading/materials for better support -- (multiple material support!)
Model *ModelManager::loadFromFile(const std::string &name)
{
	// Read texture meta data
	const auto metaLines = File::ReadAllLines(m_DataPath + "/" + name + "/meta.json");
	const auto metaSource = String::Join(metaLines, "\n");

	rapidjson::Document meta;
	meta.Parse(metaSource.c_str());
	if (meta.HasParseError())
		THROW_EXCEPTION(InvalidModelException, "Meta data parse error: %d", meta.GetParseError());

	// Get file name
	if (!meta.HasMember("name") || !meta["name"].IsString())
		THROW_EXCEPTION(InvalidModelException, "Meta data invalid name");

	// Get file extension
	if (!meta.HasMember("extension") || !meta["extension"].IsString())
		THROW_EXCEPTION(InvalidModelException, "Meta data invalid extension");

	// Get material to shader map
	if (!meta.HasMember("materialMap") || !meta["materialMap"].IsObject())
		THROW_EXCEPTION(InvalidModelException, "Meta data invalid material map");

	std::map<std::string, std::string> materialMap;
	for (auto it = meta["materialMap"].MemberBegin(); it != meta["materialMap"].MemberEnd(); ++it)
	{
		if (!it->value.IsString())
			THROW_EXCEPTION(InvalidModelException, "Meta data invalid material map value");

		materialMap.emplace(it->name.GetString(), it->value.GetString());
	}

	// Import
	Assimp::Importer importer;
	const auto filePath = m_DataPath + "/" + name + "/" + name + "." + meta["extension"].GetString();
	const auto scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		THROW_EXCEPTION(ModelLoadException, "Unable to load model %s: %s", filePath.c_str(), importer.GetErrorString());

	std::vector<Material *> materials;
	std::vector<Mesh *> meshes;

	processScene(scene, materialMap, materials, meshes);

	return New<Model>(name, meshes, materials);
}

ModelManager::ModelManager(std::string dataPath, GraphicsManager *graphicsManager)
	: m_DataPath(std::move(dataPath)), m_GraphicsManager(graphicsManager)
{
}

ModelManager::~ModelManager()
{
	// Unload models
	for (auto &pair : m_Models)
		Delete(pair.second);
	m_Models.clear();
}

Model *ModelManager::GetModel(const std::string &name)
{
	// Check if it is already loaded
	std::map<std::string, Model *>::iterator it;
	if ((it = m_Models.find(name)) != m_Models.end())
		return it->second;

	// Load model
	const auto model = loadFromFile(name);

	// Store model
	m_Models.emplace(name, model);

	return model;
}