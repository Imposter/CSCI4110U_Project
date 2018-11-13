#pragma once

#include "GraphicsManager.h"
#include "Model.h"
#include "Utility/Exception.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

DEFINE_EXCEPTION(InvalidModelException);
DEFINE_EXCEPTION(ModelLoadException);

class ModelManager
{
	std::string m_DataPath;

	GraphicsManager *m_GraphicsManager;
	std::map<std::string, Model *> m_Models;

	void loadTexture(Material *material, const std::string &path, const std::string &key, const std::string &enableKey = "");

	Mesh *processMesh(Material *material, aiMesh *mesh, const aiScene *scene);
	void processNode(std::vector<Material *> &materials, std::vector<Mesh *> &meshes, aiNode *node, 
		const aiScene *scene);
	Material *processMaterial(std::map<std::string, std::string> &materialMap, aiMaterial *material, 
		const aiScene *scene);
	void processScene(const aiScene *scene, std::map<std::string, std::string> &materialMap, 
		std::vector<Material *> &materials, std::vector<Mesh *> &meshes);
	Model *loadFromFile(const std::string &name);

public:
	ModelManager(std::string dataPath, GraphicsManager *graphicsManager);
	~ModelManager();

	// No copying/moving
	ModelManager(const ModelManager &) = delete;
	ModelManager &operator=(const ModelManager &) = delete;

	ModelManager(const ModelManager &&) = delete;
	ModelManager &operator=(const ModelManager &&) = delete;

	Model *GetModel(const std::string &name);
};