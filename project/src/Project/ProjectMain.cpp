#include "../Window.h"
#include "../Log.h"
#include "../GraphicsManager.h"
#include "../ModelManager.h"
#include "../LightManager.h"
#include "../Camera.h"
#include "../Object.h"
#include "UVSphere.h"
#include "Util.h"
#include "Animation.h"
#include "Star.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SCALE_FACTOR 10.0f
#define SCALE_STAR_FACTOR 1.0f
#define SCALE_PLANET_FACTOR 10.0f
#define SOLAR_SYSTEM_RADIUS 143730000000.0f // km
#define SCALE(x) ((x / SOLAR_SYSTEM_RADIUS) * SCALE_FACTOR)
#define SCALE_STAR(x) (SCALE(x) * SCALE_STAR_FACTOR)
#define SCALE_PLANET(x) (SCALE(x) * SCALE_PLANET_FACTOR)

// Space
const float SkyboxScale = SCALE(SOLAR_SYSTEM_RADIUS) * 2.0f;
const glm::vec3 SunPosition(0.0f, 0.0f, 0.0f);
const float SunScale = SCALE_STAR(695508.0f);
const float EarthDistance = SCALE(149600000.0f); // From sun
const float EarthScale = SCALE_PLANET(6371.0f) * 250.0f; // TODO: Test lighting
const float MoonDistance = SCALE(384400.0f) * 250.0f; // From earth
const float MoonScale = SCALE_PLANET(1737.0f) * 250.0f;

const int StarResolution = 12;
const int StarCount = 100;
const float StarInnerRadius = SCALE(250000000.0f);
const float StarOuterRadius = SCALE(500000000.0f);
const float StarMinSize = SCALE_STAR(25000.0f);
const float StarMaxSize = SCALE_STAR(50000.0f);

// Constants
const float CameraFOV = 50.0f;
const float CameraNearClip = 0.00001f;
const float CameraFarClip = -1.0f; // Infinity
const glm::vec4 CameraClearColor(0.0f, 0.0f, 0.0f, 1.0f);
const float CameraClearDepth = 1.0f;
const float CameraMoveSensitivity = SCALE(0.1f);
const float CameraZoomSensitivity = EarthDistance / 100.0f; // TODO: Find out direction -- if we change position to positive earth distance it works...
const glm::vec3 CameraDefaultPosition(0.0f, EarthDistance * 2.0f, EarthDistance * 2.0f);//-EarthDistance); //const glm::vec3 CameraDefaultPosition(0.0f, EarthDistance * 4.0f, EarthDistance * 4.0f); // negative or positive?

// Ship
const float ShipScale = SCALE(10.0f); // km
const glm::vec3 ShipStartPosition(0.0f, 0.0f, 0.0f); // TODO: Set starting position

// Vars
unsigned int g_Width;
unsigned int g_Height;

Window *g_RootWindow;
GraphicsManager *g_GraphicsManager;
ModelManager *g_ModelManager;
LightManager *g_LightManager;

Camera *g_Camera;
Object *g_RootObject;
Node *g_RootNode;

// Shaders
Shader *g_FlatShader;
Shader *g_FakeSkyboxShader;
Shader *g_LightShader;

// Lights
PointLight *g_PointLight;

// Meshes
Mesh *g_StarMesh;

// Materials
Material *g_StarMaterial;

// Models
Model *g_SkyboxModel;
Model *g_SunModel;
Model *g_EarthModel;
Model *g_MoonModel;
Model *g_ShipModel;

// Animations
Animation *g_TestAnimation;

// Camera
unsigned int g_MouseLastX = 0;
unsigned int g_MouseLastY = 0;
bool g_EyeMove = false;
Transform *g_LookTarget;

// For stars
void CreateSphereMesh(int resolution, Shader *matShader, Mesh **outMesh, Material **outMaterial)
{
	// Generate mesh
	const UVSphere sphere(resolution, resolution, 1.0f);

	// Create material
	*outMaterial = New<Material>("Material", matShader);

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
	*outMesh = New<Mesh>("Sphere", vertices, indices, *outMaterial);
}

void CreateScene()
{
	// Create camera
	g_Camera = New<Camera>(CameraFOV, CameraNearClip, CameraFarClip, 
		static_cast<float>(g_RootWindow->GetWidth()) / static_cast<float>(g_RootWindow->GetHeight()), g_GraphicsManager);
	g_Camera->SetClearMode(kCameraClearMode_Both);
	g_Camera->SetClearColor(CameraClearColor);
	g_Camera->SetClearDepth(CameraClearDepth);

	// Set default camera position
	const auto cameraTransform = g_Camera->GetTransform();
	cameraTransform->SetPosition(CameraDefaultPosition);
	cameraTransform->LookAt(SunPosition);

	// Add shaders to camera
	g_Camera->AddShader(g_FlatShader);
	g_Camera->AddShader(g_FakeSkyboxShader);
	g_Camera->AddShader(g_LightShader);

	// Create point light
	g_PointLight = g_LightManager->CreateLight<PointLight>();
	g_PointLight->SetAmbient(glm::vec3(0.25f));
	g_PointLight->SetDiffuse(glm::vec3(0.8f));
	g_PointLight->SetSpecular(glm::vec3(0.5f));
	g_PointLight->SetIntensity(1.0f);
	g_PointLight->SetPosition(SunPosition);

	// Create skybox
	g_SkyboxModel = g_ModelManager->LoadModel("Skybox");
	//g_RootNode->AddChild(g_SkyboxModel); // TODO: Testing camera look at

	const auto skyboxTransform = g_SkyboxModel->GetTransform();
	skyboxTransform->SetScale(glm::vec3(SkyboxScale));

	// Create sun
	g_SunModel = g_ModelManager->LoadModel("Sun");
	g_RootNode->AddChild(g_SunModel); // TODO/NOTE: Not rendering on purpose

	// Set sun transform
	const auto sunTransform = g_SunModel->GetTransform();
	sunTransform->SetScale(glm::vec3(SunScale));
	sunTransform->SetPosition(SunPosition);

	// Create earth
	g_EarthModel = g_ModelManager->LoadModel("Earth");
	//g_RootNode->AddChild(g_EarthModel);

	// Set earth transform
	const auto earthTransform = g_EarthModel->GetTransform();
	earthTransform->SetParent(sunTransform);
	earthTransform->SetScale(glm::vec3(EarthScale));

	// Create moon
	g_MoonModel = g_ModelManager->LoadModel("Moon");
	//g_RootNode->AddChild(g_MoonModel); // TODO/NOTE: Test

	// Set moon transform
	const auto moonTransform = g_MoonModel->GetTransform();
	moonTransform->SetParent(earthTransform);
	moonTransform->SetScale(glm::vec3(MoonScale));

	// Create star mesh
	CreateSphereMesh(StarResolution, g_FlatShader, &g_StarMesh, &g_StarMaterial);
	g_StarMaterial->GetShader()->Use();
	g_StarMaterial->GetVariable(kMaterialVar_Diffuse)->SetVec3(glm::vec3(0.8f, 0.8f, 0.8f));

	// Generate star field
	//CreateStarField(StarCount, StarInnerRadius, StarOuterRadius, StarMinSize, StarMaxSize, g_StarMesh, g_StarMaterial, g_RootObject, g_RootNode);

	// Create ship
	g_ShipModel = g_ModelManager->LoadModel("Ship1");
	//g_RootNode->AddChild(g_ShipModel); // TODO/NOTE: Update transform

	// Set ship transform
	const auto shipTransform = g_ShipModel->GetTransform();
	shipTransform->SetScale(glm::vec3(ShipScale)); // 100 meters -- TODO: Currently very small, we might need to scale it up
	shipTransform->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(180.0f));
	shipTransform->SetPosition(ShipStartPosition);

	// TODO/NOTE: Testing animations
	g_TestAnimation = New<Animation>(shipTransform);
	g_TestAnimation->AddKeyFrame(Animation::KeyFrame(1000.0f, Animation::kAxis_Forward, glm::radians(360.0f), Animation::kAxis_Forward, 10000.0f));
	g_TestAnimation->AddKeyFrame(Animation::KeyFrame(0.0f, Animation::kAxis_Forward, glm::radians(360.0f), Animation::kAxis_Right, 5000.0f));

	// Set initial look target as sun
	g_LookTarget = sunTransform;
}

void Project_Update(float time, float deltaTime)
{
	// Get time in seconds
	const auto timeSeconds = time / 1000.0f;

	// Orbit == 365 days
	// Orbit scale factor = 360 / Orbit
	// Time In Seconds to Orbit = 31536000

	// TODO: Add time scale factor for rotating and shit

	// Rotate sun on its own axis
	// TODO: ...

	// Rotate earth around sun
	const auto earthTransform = g_EarthModel->GetTransform();
	earthTransform->SetPosition(glm::vec3(EarthDistance * sin(timeSeconds), 0.0f, EarthDistance * cos(timeSeconds)));
	
	// Rotate earth on its own axis
	earthTransform->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), timeSeconds * 2.0f);

	// Rotate moon around earth
	const auto moonTransform = g_MoonModel->GetTransform();
	//moonTransform->SetPosition(glm::vec3(MoonDistance * sin(timeSeconds), 0.0f, MoonDistance * cos(timeSeconds)));

	// Rotate moon on its own axis
	//moonTransform->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), timeSeconds * 2.0f);
	
	// Translate ship
	const auto shipTransform = g_ShipModel->GetTransform();
	//shipTransform->OffsetRotation(shipTransform->Right(), glm::radians(1.0f));
	//shipTransform->LookAt(earthTransform->GetPosition());
	//shipTransform->OffsetPosition(-shipTransform->Forward() * 0.01f);

	// Set camera transform to ship transform with offset
	const auto cameraTransform = g_Camera->GetTransform();
	// test
	//cameraTransform->OffsetPosition(cameraTransform->Right() * SCALE(1000000.0f));
	cameraTransform->LookAt(g_LookTarget->GetPosition());


	// TODO/NOTE: Testing
	//g_TestAnimation->Update(time);

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

	// Apply lighting
	g_LightManager->Apply(g_LightShader, g_Camera->GetTransform()->GetPosition());
	
	// Render camera and nodes
	g_Camera->Render(g_RootNode, deltaTime);
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

void Project_WindowMouseWheel(MouseEventArgs &args)
{
	// Offset camera position
	const auto transform = g_Camera->GetTransform();
	transform->OffsetPosition(transform->Forward() * (CameraZoomSensitivity * -args.Delta));//TODO: This shouldn't be negative because our axis are messed up?
}

// TODO/NOTE: This is busted
void Project_WindowMouseMove(MouseEventArgs &args)
{
	if (g_EyeMove)
	{
		// Get change in position
		const auto deltaX = args.X - g_MouseLastX; // X
		const auto deltaY = args.Y - g_MouseLastY; // Z

		const auto x = static_cast<float>(deltaX) / g_Width;
		const auto y = static_cast<float>(deltaY) / g_Height;

		// Offset camera position
		const auto transform = g_Camera->GetTransform();
		transform->OffsetPosition(transform->Right() * (CameraMoveSensitivity * x));
		transform->OffsetPosition(transform->Up() * (CameraMoveSensitivity * -y));
	}

	// Update last mouse position
	g_MouseLastX = args.X;
	g_MouseLastY = args.Y;
}

void Project_WindowMouseDown(MouseEventArgs &args)
{
	if (args.Button == kMouseButton_Left)
		g_EyeMove = true;
}

void Project_WindowMouseUp(MouseEventArgs &args)
{
	if (args.Button == kMouseButton_Right)
	{
		g_EyeMove = false;
		g_MouseLastX = 0;
		g_MouseLastY = 0;
	}
}

bool Project_Initialize(Window *window)
{
	// Store window
	g_RootWindow = window;

	// Setup window events
	window->OnResize.Add(Project_WindowResize);
	window->OnKeyDown.Add(Project_WindowKeyDown);
	window->OnKeyPress.Add(Project_WindowKeyPress);
	window->OnMouseMove.Add(Project_WindowMouseMove);
	window->OnMouseWheel.Add(Project_WindowMouseWheel);
	window->OnMouseDown.Add(Project_WindowMouseDown);
	window->OnMouseUp.Add(Project_WindowMouseUp);

	// Seed random with time
	srand(time(nullptr));

	// Create graphics manager
	g_GraphicsManager = New<GraphicsManager>("data");

	try
	{
		// Create model manager
		g_ModelManager = New<ModelManager>("data/models", g_GraphicsManager);

		// Create light manager
		g_LightManager = New<LightManager>();

		// Create root object
		g_RootObject = New<Object>("Root");

		// Create root node
		g_RootNode = New<Node>("Root");

		// Get flat shader
		g_FlatShader = g_GraphicsManager->GetShader("Flat");

		// Get fake skybox shader
		g_FakeSkyboxShader = g_GraphicsManager->GetShader("FakeSkybox");

		// Get lambert shader
		g_LightShader = g_GraphicsManager->GetShader("Light");

		// Create scene
		CreateScene();
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
	// TODO/NOTE: Testing
	Delete(g_TestAnimation);

	// Shutdown
	Delete(g_StarMesh);
	Delete(g_StarMaterial);

	Delete(g_RootNode);
	Delete(g_RootObject);
	Delete(g_Camera);
	Delete(g_LightManager);
	Delete(g_ModelManager);
	Delete(g_GraphicsManager);

	return true;
}