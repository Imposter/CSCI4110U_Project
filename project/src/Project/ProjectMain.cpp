#include "../Window.h"
#include "../Log.h"
#include "../GraphicsManager.h"
#include "../ModelManager.h"
#include "../Camera.h"
#include "../Object.h"
#include "Star.h"

// TEST
#include "UVSphere.h"
#include "Util.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Constants
const float CameraFOV = 60.0f;
const float CameraNearClip = 0.1f;
const float CameraFarClip = 1000000.0f;
const glm::vec4 CameraClearColor(0.0f, 0.0f, 0.0f, 1.0f);

const glm::vec3 Up(0.0f, 1.0f, 0.0f);
const glm::vec3 LookTarget(0.0f, 0.0f, 0.0f);
const glm::vec3 EyePosition(10.0, 10.0f, 10.0f);

const int StarCount = 25;//100000;
const float StarInnerRadius = 0.0f;//500.0f;
const float StarOuterRadius = 5.0f;//100000.0f;
const float StarMinSize = 1.0f;//10.0f;
const float StarMaxSize = 10.0f;//100.0f;

// Vars
unsigned int g_Width;
unsigned int g_Height;

GraphicsManager *g_GraphicsManager;
ModelManager *g_ModelManager;

Camera *g_Camera;
Object *g_RootObject;
Node *g_RootNode;

Shader *g_FlatShader;

// TEST
Model *g_Model;

void GenMesh()
{
	// Load model
	// TODO/NOTE: We've changed shaders for this to flat in meta.json
	g_Model = g_ModelManager->GetModel("Ship1");
	const auto t = g_Model->GetTransform(); // NODES werent meant to have transforms because now we cant share models, but oh well? -- unless we remove Model from node class and pass a transform to model before rendering?
	t->SetScale(glm::vec3(10.0f));

	/*
	// Generate mesh
	const UVSphere sphere(STAR_RESOLUTION, STAR_RESOLUTION, 1.0f);

	// Create material
	const auto material = New<Material>("StarMaterial", g_FlatShader); // TODO: Leak

	const glm::vec3 color(1.0f, 0.0f, 0.0f);

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
	g_RootNode->CreateChild<Mesh>("Star", vertices, indices, material);
	*/
}

void Project_Update(float time, float deltaTime)
{
	// Update objects
	g_RootObject->Update(time, deltaTime);

	// Update camera
	g_Camera->Update(deltaTime);

	// ...
}

void Project_Render(float time, float deltaTime)
{
	// "Render" objects
	g_RootObject->Render(time, deltaTime);

	// Render camera and nodes
	//g_Camera->Render(g_RootNode, deltaTime);
	g_Camera->Render(g_Model, deltaTime);
}

// Window events
void Project_WindowResize(ResizeEventArgs &args)
{
	// Divide by zero
	if (args.Width == 0 || args.Height == 0)
		return;

	// Update projection matrix
	const auto aspectRatio = static_cast<float>(args.Width) / static_cast<float>(args.Height);
	g_Camera->SetAspectRatio(aspectRatio);

	glViewport(0, 0, args.Width, args.Height);

	// Store values
	g_Width = args.Width;
	g_Height = args.Height;
}

void Project_WindowKeyDown(KeyEventArgs &args)
{
	if (args.Value == kKey_Escape)
	{
		// Exit
		args.Target->Close();
	}
}

void Project_WindowKeyPress(KeyPressEventArgs &args)
{

}

bool Project_Initialize(Window *window)
{
	// Setup window events
	window->OnResize.Add(Project_WindowResize);
	window->OnKeyDown.Add(Project_WindowKeyDown);
	window->OnKeyPress.Add(Project_WindowKeyPress);

	// Seed random with time
	srand(time(nullptr));

	// Create graphics manager
	g_GraphicsManager = New<GraphicsManager>("data");

	try
	{
		// Create model manager
		g_ModelManager = New<ModelManager>("data/models", g_GraphicsManager);

		// Create camera
		g_Camera = New<Camera>(CameraFOV, CameraNearClip, CameraFarClip, static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()), g_GraphicsManager);
		g_Camera->SetClearMode(kCameraClearMode_Color);
		g_Camera->SetClearColor(CameraClearColor);
		g_Camera->LookAt(LookTarget, EyePosition, Up);

		// Create root object
		g_RootObject = New<Object>("Root");

		// Create root node
		g_RootNode = New<Node>("Root");

		// Get flat shader
		g_FlatShader = g_GraphicsManager->GetShader("Flat");
		// TODO: Use shader -- we have errors for some unknown reason
		g_FlatShader->Use();

		// Generate star field
		//GenerateStars(StarCount, StarInnerRadius, StarOuterRadius, StarMinSize, StarMaxSize, g_FlatShader, g_RootObject, g_RootNode);

		// TODO: Test gen star
		GenMesh();

		// TODO: Other shizzle
	}
	catch (ShaderCompileException &ex)
	{
		LOG_TRACE("Project", ex.what());

		// No cleanup
		return false;
	}
	catch (ModelLoadException &ex)
	{
		LOG_TRACE("Project", ex.what());

		// No cleanup
		return false;
	}

	return true;
}

bool Project_Shutdown()
{
	// Shutdown
	Delete(g_RootNode);
	Delete(g_RootObject);
	Delete(g_Camera);
	Delete(g_ModelManager);
	Delete(g_GraphicsManager);

	return true;
}