#include "ModelUtil.h"
#include "Memory.h"
#include "Utility/FileUtil.h"
#include <rapidjson/document.h>
#define TINYOBJLOADER_IMPLEMENTATION 
#include <tiny_obj_loader.h>

enum ModelFormat
{
	kModelFormat_Wavefront,
};

Model *LoadWavefrontModelFromFile(const std::string &path, const std::string &name, const std::string &extension)
{
	tinyobj::attrib_t objAttribs;
	std::vector<tinyobj::shape_t> objShapes;
	std::vector<tinyobj::material_t> objMaterials;

	std::string warnings;
	std::string errors;
	const auto objPath = path + "/" + name + "." + extension;
	if (!LoadObj(&objAttribs, &objShapes, &objMaterials, &warnings, &errors, objPath.c_str(), path.c_str(), true))
		THROW_EXCEPTION(ModelLoadException, "Unable to load model %s: Error: %s | Warning: %s", objPath.c_str(),
			errors.c_str(), warnings.c_str());

	// TODO: Make material a class that takes values, then applies them to a shader...
	std::vector<Material *> materials;
	for (auto &m : objMaterials)
	{

	}

	// Load meshes
	std::vector<Mesh *> meshes;
	for (auto &s : objShapes)
	{
		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;

		// For each face
		size_t offset = 0;
		for (size_t f = 0; f < s.mesh.num_face_vertices.size(); f++) 
		{
			const auto fv = s.mesh.num_face_vertices[f];

			// For each face vertex
			for (size_t v = 0; v < fv; v++) 
			{
				const auto index = s.mesh.indices[offset + v];

				MeshVertex vertex;
				vertex.Position.x = objAttribs.vertices[3 * index.vertex_index + 0];
				vertex.Position.y = objAttribs.vertices[3 * index.vertex_index + 1];
				vertex.Position.z = objAttribs.vertices[3 * index.vertex_index + 2];
				vertex.Normal.x = objAttribs.vertices[3 * index.normal_index + 0];
				vertex.Normal.y = objAttribs.vertices[3 * index.normal_index + 1];
				vertex.Normal.z = objAttribs.vertices[3 * index.normal_index + 2];
				vertex.Colour.x = objAttribs.colors[3 * index.vertex_index + 0];
				vertex.Colour.y = objAttribs.colors[3 * index.vertex_index + 1];
				vertex.Colour.z = objAttribs.colors[3 * index.vertex_index + 2];

				// Optional textures
				if (index.texcoord_index > -1)
				{
					vertex.TexCoords.x = objAttribs.vertices[2 * index.texcoord_index + 0];
					vertex.TexCoords.y = objAttribs.vertices[2 * index.texcoord_index + 1];
				}

				vertices.push_back(vertex);
				indices.push_back(offset + v);
			}
			offset += fv;

			// per-face material ... we're not doing that
			//shapes[s].mesh.material_ids[f]; // TODO: ...
		}

		// Build mesh
		meshes.push_back(New<Mesh>(s.name, vertices, indices));
		/*
		// Build vertices
		std::vector<MeshVertex> vertices;
		for (auto &i : s.mesh.indices)
		{
			MeshVertex v;
			v.Position.x = objAttribs.vertices[3 * i.vertex_index + 0];
			v.Position.y = objAttribs.vertices[3 * i.vertex_index + 1];
			v.Position.z = objAttribs.vertices[3 * i.vertex_index + 2];
			v.Normal.x = objAttribs.vertices[3 * i.normal_index + 0];
			v.Normal.y = objAttribs.vertices[3 * i.normal_index + 1];
			v.Normal.z = objAttribs.vertices[3 * i.normal_index + 2];
			v.Colour.x = objAttribs.colors[3 * i.vertex_index + 0];
			v.Colour.y = objAttribs.colors[3 * i.vertex_index + 1];
			v.Colour.z = objAttribs.colors[3 * i.vertex_index + 2];

			// Optional textures
			if (i.texcoord_index > -1)
			{
				v.TexCoords.x = objAttribs.vertices[2 * i.texcoord_index + 0];
				v.TexCoords.y = objAttribs.vertices[2 * i.texcoord_index + 1];
			}

			vertices.push_back(v);
		}

		// Build indices
		std::vector<unsigned int> indices;
		indices.reserve(vertices.size());
		for (unsigned int i = 0; i < vertices.size(); i++)
			indices.push_back(i);

		// Build mesh
		meshes.push_back(New<Mesh>(s.name, vertices, indices));
		*/
	}

	return New<Model>(name, meshes, materials);
}

Model *LoadModelFromFile(const std::string &path, const std::string &name)
{
	// Read texture meta data
	const auto metaLines = File::ReadAllLines(path + "/" + name + ".json");
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

	// Get format
	if (!meta.HasMember("format") || !meta["format"].IsInt())
		THROW_EXCEPTION(InvalidModelException, "Meta data invalid format");

	// Determine format
	Texture::Format format;
	switch (meta["format"].GetInt())
	{
	case kModelFormat_Wavefront:
		return LoadWavefrontModelFromFile(path, meta["name"].GetString(), meta["extension"].GetString());
	default:
		THROW_EXCEPTION(InvalidModelException, "Meta data invalid unknown format");
	}
}

void DestroyModel(Model *m)
{
	Delete(m);
}