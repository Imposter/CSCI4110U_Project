#include "../Window.h"
#include "../Log.h"
#include "../GraphicsManager.h"
#include "../ModelManager.h"
#include "../LightManager.h"
#include "../Camera.h"
#include "../Object.h"
#include "UVSphere.h"
#include "Util.h"
#include "Star.h"
#include "Animation.h"

#define NO_SKYBOX
#define NO_PLANETS

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SOLAR_SYSTEM_RADIUS 350.0f
#define GET_DISTANCE(x) (x * SOLAR_SYSTEM_RADIUS)

// Space
#ifndef NO_SKYBOX
const float SkyboxScale = 5000.0f;
#endif
const glm::vec3 SunPosition(0.0f, 0.0f, 0.0f);
const float SunScale = 10.0f;
#ifndef NO_PLANETS
const float MercuryDistance = GET_DISTANCE(0.1f); // From sun
const float MercuryScale = 0.5f;
const float MercurySpeed = 4.0f;
const float VenusDistance = GET_DISTANCE(0.15f); // From sun
const float VenusScale = 1.5f;
const float VenusSpeed = 2.0f;
const float EarthDistance = GET_DISTANCE(0.2f); // From sun
const float EarthScale = 1.0f;
const float EarthSpeed = 1.0f;
const float MarsDistance = GET_DISTANCE(0.25f); // From sun
const float MarsScale = 1.0f;
const float MarsSpeed = 0.75f;
const float JupiterDistance = GET_DISTANCE(0.4f); // From sun
const float JupiterScale = 3.0f;
const float JupiterSpeed = 0.2f;
const float SaturnDistance = GET_DISTANCE(0.5f); // From sun
const float SaturnScale = 2.5f;
const float SaturnSpeed = 0.1f;
const float UranusDistance = GET_DISTANCE(0.6f); // From sun
const float UranusScale = 2.75f;
const float UranusSpeed = 0.075f;
const float NeptuneDistance = GET_DISTANCE(0.7f); // From sun
const float NeptuneScale = 2.7f;
const float NeptuneSpeed = 0.075f;
#endif

const int StarResolution = 12;
const int StarCount = 100;
const float StarInnerRadius = SOLAR_SYSTEM_RADIUS * 1.00f;
const float StarOuterRadius = SOLAR_SYSTEM_RADIUS * 1.25f;
const float StarMinSize = 0.5f;
const float StarMaxSize = 2.0f;

// Constants
const float CameraFOV = 50.0f;
const float CameraNearClip = 0.01f;
const float CameraFarClip = 10000.0f; // Infinity
const glm::vec4 CameraClearColor(0.0f, 0.0f, 0.0f, 1.0f);
const float CameraClearDepth = 1.0f;
const float CameraZoomDefault = SOLAR_SYSTEM_RADIUS / 4;
const float CameraZoomInLimit = SOLAR_SYSTEM_RADIUS / 32;
const float CameraZoomOutLimit = SOLAR_SYSTEM_RADIUS * 1.5f;
const float CameraZoomSensitivity = 5.0f;
const float CameraSpeed = 0.5f;

// Ship
const float ShipScale = 0.0025f;
const float ShipStartPosition = GET_DISTANCE(1.0f);

// Vars
unsigned int g_Width;
unsigned int g_Height;
float g_LastTime;

Window *g_RootWindow;
GraphicsManager *g_GraphicsManager;
ModelManager *g_ModelManager;
LightManager *g_LightManager;

Camera *g_Camera;
Object *g_RootObject;
Node *g_RootNode;

// Shaders
Shader *g_FlatShader;
#ifndef NO_SKYBOX
Shader *g_FakeSkyboxShader;
#endif
Shader *g_LightShader;

// Lights
PointLight *g_SunLight;

// Models
std::vector<Model *> g_Models;
#ifndef NO_SKYBOX
Model *g_SkyboxModel;
#endif
Model *g_SunModel;
#ifndef NO_PLANETS
Model *g_MercuryModel;
Model *g_VenusModel;
Model *g_EarthModel;
Model *g_MarsModel;
Model *g_JupiterModel;
Model *g_SaturnModel;
Model *g_UranusModel;
Model *g_NeptuneModel;
#endif
Model *g_ShipModel;

// Stars
Mesh *g_StarMesh;
Material *g_StarMaterial;

// Camera
int g_LookTarget;
bool g_CameraRotating;
float g_CameraRotation;
float g_CameraZoom = CameraZoomDefault;

// Orbits and rotation
#ifndef NO_PLANETS
float g_MercuryRotation;
float g_VenusRotation;
float g_EarthRotation;
float g_MarsRotation;
float g_JupiterRotation;
float g_SaturnRotation;
float g_UranusRotation;
float g_NeptuneRotation;
#endif

// Animations
AnimationPlayer *g_AnimationPlayer;

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

	// Add shaders to camera
	g_Camera->AddShader(g_FlatShader);
	g_Camera->AddShader(g_LightShader);
#ifndef NO_SKYBOX
	g_Camera->AddShader(g_FakeSkyboxShader);
#endif

#ifndef NO_SKYBOX
	// Create skybox
	g_SkyboxModel = g_ModelManager->LoadModel("Skybox");
	g_RootNode->AddChild(g_SkyboxModel);

	const auto skyboxTransform = g_SkyboxModel->GetTransform();
	skyboxTransform->SetScale(glm::vec3(SkyboxScale));

	LOG_TRACE("Sim", "Skybox loaded");
#endif

	// Create sun
	g_SunModel = g_ModelManager->LoadModel("Sun");
	g_Models.push_back(g_SunModel);
	g_RootNode->AddChild(g_SunModel);

	// Set transform
	const auto sunTransform = g_SunModel->GetTransform();
	sunTransform->SetScale(glm::vec3(SunScale));
	sunTransform->SetPosition(SunPosition);	

	// Create point light (at sun)
	g_SunLight = g_LightManager->CreateLight<PointLight>();
	g_SunLight->SetAmbient(glm::vec3(0.5f));
	g_SunLight->SetDiffuse(glm::vec3(0.8f));
	g_SunLight->SetSpecular(glm::vec3(0.5f));
	g_SunLight->SetIntensity(1.0f);
	g_SunLight->SetPosition(SunPosition);

	LOG_TRACE("Sim", "Sun loaded");

#ifndef NO_PLANETS
	// Create mercury
	g_MercuryModel = g_ModelManager->LoadModel("Mercury");
	g_Models.push_back(g_MercuryModel);
	g_RootNode->AddChild(g_MercuryModel);

	// Set transform
	const auto mercuryTransform = g_MercuryModel->GetTransform();
	mercuryTransform->SetParent(sunTransform);
	mercuryTransform->SetScale(glm::vec3(MercuryScale));

	LOG_TRACE("Sim", "Mercury loaded");

	// Create venus
	g_VenusModel = g_ModelManager->LoadModel("Venus");
	g_Models.push_back(g_VenusModel);
	g_RootNode->AddChild(g_VenusModel);

	// Set transform
	const auto venusTransform = g_VenusModel->GetTransform();
	venusTransform->SetParent(sunTransform);
	venusTransform->SetScale(glm::vec3(VenusScale));

	LOG_TRACE("Sim", "Venus loaded");

	// Create earth
	g_EarthModel = g_ModelManager->LoadModel("Earth");
	g_Models.push_back(g_EarthModel);
	g_RootNode->AddChild(g_EarthModel);

	// Set transform
	const auto earthTransform = g_EarthModel->GetTransform();
	earthTransform->SetParent(sunTransform);
	earthTransform->SetScale(glm::vec3(EarthScale));

	LOG_TRACE("Sim", "Earth loaded");

	// Create mars
	g_MarsModel = g_ModelManager->LoadModel("Mars");
	g_Models.push_back(g_MarsModel);
	g_RootNode->AddChild(g_MarsModel);

	// Set transform
	const auto marsTransform = g_MarsModel->GetTransform();
	marsTransform->SetParent(sunTransform);
	marsTransform->SetScale(glm::vec3(MarsScale));

	LOG_TRACE("Sim", "Mars loaded");

	// Create jupiter
	g_JupiterModel = g_ModelManager->LoadModel("Jupiter");
	g_Models.push_back(g_JupiterModel);
	g_RootNode->AddChild(g_JupiterModel);

	// Set transform
	const auto jupiterTransform = g_JupiterModel->GetTransform();
	jupiterTransform->SetParent(sunTransform);
	jupiterTransform->SetScale(glm::vec3(JupiterScale));

	LOG_TRACE("Sim", "Jupiter loaded");

	// Create saturn
	g_SaturnModel = g_ModelManager->LoadModel("Saturn");
	g_Models.push_back(g_SaturnModel);
	g_RootNode->AddChild(g_SaturnModel);

	// Set transform
	const auto saturnTransform = g_SaturnModel->GetTransform();
	saturnTransform->SetParent(sunTransform);
	saturnTransform->SetScale(glm::vec3(SaturnScale));

	LOG_TRACE("Sim", "Saturn loaded");

	// Create uranus
	g_UranusModel = g_ModelManager->LoadModel("Uranus");
	g_Models.push_back(g_UranusModel);
	g_RootNode->AddChild(g_UranusModel);

	// Set transform
	const auto uranusTransform = g_UranusModel->GetTransform();
	uranusTransform->SetParent(sunTransform);
	uranusTransform->SetScale(glm::vec3(UranusScale));

	LOG_TRACE("Sim", "Uranus loaded");

	// Create neptune
	g_NeptuneModel = g_ModelManager->LoadModel("Neptune");
	g_Models.push_back(g_NeptuneModel);
	g_RootNode->AddChild(g_NeptuneModel);

	// Set transform
	const auto neptuneTransform = g_NeptuneModel->GetTransform();
	neptuneTransform->SetParent(sunTransform);
	neptuneTransform->SetScale(glm::vec3(NeptuneScale));

	LOG_TRACE("Sim", "Neptune loaded");
#endif

	// Create ship
	g_ShipModel = g_ModelManager->LoadModel("Ship");
	g_Models.push_back(g_ShipModel);
	g_RootNode->AddChild(g_ShipModel);

	// Set transform
	const auto shipTransform = g_ShipModel->GetTransform();
	shipTransform->SetPosition(glm::vec3(0.0f, 0.0f, -ShipStartPosition));
	shipTransform->SetScale(glm::vec3(ShipScale));

	// Initialize animations
	g_AnimationPlayer = New<AnimationPlayer>();

	// Create animation group 1 -> Move ship forward toward center of solar system, while simultaneously rotating right
	{
		// Create container
		const auto container = New<AnimationContainer>();

		// Create animations
		const auto a1 = New<Animation>(shipTransform);
		a1->AddKeyFrame(Animation::KeyFrame(GET_DISTANCE(1.0f), Animation::kAxis_Backward, glm::radians(360.0f), Animation::kAxis_Forward, 10000.0f));

		// Store animations in container
		container->AddAnimation(a1);

		// Add container to player
		g_AnimationPlayer->AddContainer(container);
	}

	LOG_TRACE("Sim", "Ship loaded");
	
	// Create star mesh
	CreateSphereMesh(StarResolution, g_FlatShader, &g_StarMesh, &g_StarMaterial);
	g_StarMaterial->GetShader()->Use();
	g_StarMaterial->GetVariable(kMaterialVar_Diffuse)->SetVec3(glm::vec3(0.8f, 0.8f, 0.8f));

	// Generate star field
	CreateStarField(StarCount, StarInnerRadius, StarOuterRadius, StarMinSize, StarMaxSize, g_StarMesh, g_StarMaterial, g_RootObject, g_RootNode);

	LOG_TRACE("Sim", "Generated star field");

	// Set initial look target as sun
	g_LookTarget = 0;

	// Set camera as rotating
	g_CameraRotating = true;
}

void Project_Update(float time, float deltaTime)
{
	// Get time in seconds
	const auto timeSeconds = time / 1000.0f;
	const auto deltaTimeSeconds = deltaTime / 1000.0f;

	// Update sun
	{
		// Update transform
		const auto sunTransform = g_SunModel->GetTransform();

		// Rotate sun on its own axis
		sunTransform->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), timeSeconds * 0.1f);
	}

#ifndef NO_PLANETS
	// Update mercury
	{
		// Update rotation
		g_MercuryRotation += MercurySpeed * deltaTimeSeconds;

		// Update transform
		const auto mercuryTransform = g_MercuryModel->GetTransform();
		mercuryTransform->SetPosition(glm::vec3(MercuryDistance * sin(g_MercuryRotation), 0.0f, MercuryDistance * cos(g_MercuryRotation)));
	}

	// Update venus
	{
		// Update rotation
		g_VenusRotation += VenusSpeed * deltaTimeSeconds;

		// Update transform
		const auto venusTransform = g_VenusModel->GetTransform();
		venusTransform->SetPosition(glm::vec3(VenusDistance * sin(g_VenusRotation), 0.0f, VenusDistance * cos(g_VenusRotation)));
	}

	// Update earth
	{
		// Update rotation
		g_EarthRotation += EarthSpeed * deltaTimeSeconds;

		// Update transform
		const auto earthTransform = g_EarthModel->GetTransform();
		earthTransform->SetPosition(glm::vec3(EarthDistance * sin(g_EarthRotation), 0.0f, EarthDistance * cos(g_EarthRotation)));
	}

	// Update mars
	{
		// Update rotation
		g_MarsRotation += MarsSpeed * deltaTimeSeconds;

		// Update transform
		const auto marsTransform = g_MarsModel->GetTransform();
		marsTransform->SetPosition(glm::vec3(MarsDistance * sin(g_MarsRotation), 0.0f, MarsDistance * cos(g_MarsRotation)));
	}

	// Update jupiter
	{
		// Update rotation
		g_JupiterRotation += JupiterSpeed * deltaTimeSeconds;

		// Update transform
		const auto jupiterTransform = g_JupiterModel->GetTransform();
		jupiterTransform->SetPosition(glm::vec3(JupiterDistance * sin(g_JupiterRotation), 0.0f, JupiterDistance * cos(g_JupiterRotation)));
	}

	// Update saturn
	{
		// Update rotation
		g_SaturnRotation += SaturnSpeed * deltaTimeSeconds;

		// Update transform
		const auto saturnTransform = g_SaturnModel->GetTransform();
		saturnTransform->SetPosition(glm::vec3(SaturnDistance * sin(g_SaturnRotation), 0.0f, SaturnDistance * cos(g_SaturnRotation)));
	}

	// Update uranus
	{
		// Update rotation
		g_UranusRotation += UranusSpeed * deltaTimeSeconds;

		// Update transform
		const auto uranusTransform = g_UranusModel->GetTransform();
		uranusTransform->SetPosition(glm::vec3(UranusDistance * sin(g_UranusRotation), 0.0f, UranusDistance * cos(g_UranusRotation)));
	}

	// Update neptune
	{
		// Update rotation
		g_NeptuneRotation += NeptuneSpeed * deltaTimeSeconds;

		// Update transform
		const auto neptuneTransform = g_NeptuneModel->GetTransform();
		neptuneTransform->SetPosition(glm::vec3(NeptuneDistance * sin(g_NeptuneRotation), 0.0f, NeptuneDistance * cos(g_NeptuneRotation)));
	}
#endif

	// Update animations
	g_AnimationPlayer->Update(time);

	// Set camera transform
	const auto targetModel = g_Models[g_LookTarget];
	const auto targetTransform = targetModel->GetTransform();
	const auto cameraTransform = g_Camera->GetTransform();

	if (g_CameraRotating)
		g_CameraRotation += CameraSpeed * deltaTimeSeconds;
	
	cameraTransform->SetPosition(targetTransform->GetPosition() + glm::vec3(g_CameraZoom * sin(g_CameraRotation), g_CameraZoom * cos(g_CameraRotation), -g_CameraZoom));
	const auto cameraMatrix = glm::lookAt(cameraTransform->GetPosition(), targetTransform->GetPosition(), glm::vec3(0.0f, 1.0f, 0.0f));
	cameraTransform->SetMatrix(cameraMatrix);

	// Update objects
	g_RootObject->Update(time, deltaTime);

	// Update camera
	g_Camera->Update(deltaTime);

	// Update last time
	g_LastTime = time;
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
	else if (args.Value == kKey_Left)
	{
		if (--g_LookTarget < 0)
			g_LookTarget = g_Models.size() - 1;
	}
	else if (args.Value == kKey_Right)
	{
		if (++g_LookTarget >= g_Models.size())
			g_LookTarget = 0;
	}
}

void Project_WindowKeyPress(KeyPressEventArgs &args)
{
	if (args.Char == 'c')
		g_CameraRotating = !g_CameraRotating;
	if (args.Char == 'r')
		g_AnimationPlayer->Reset(g_LastTime); // TODO: Write instruction for this
}

void Project_WindowMouseWheel(MouseEventArgs &args)
{
	g_CameraZoom += -args.Delta * CameraZoomSensitivity;
	if (g_CameraZoom < CameraZoomInLimit)
		g_CameraZoom = CameraZoomInLimit;
	if (g_CameraZoom > CameraZoomOutLimit)
		g_CameraZoom = CameraZoomOutLimit;
}

void Project_WindowMouseMove(MouseEventArgs &args)
{
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

#ifndef NO_SKYBOX
		// Get fake skybox shader
		g_FakeSkyboxShader = g_GraphicsManager->GetShader("FakeSkybox");
#endif

		// Get lambert shader
		g_LightShader = g_GraphicsManager->GetShader("Light");

		// Print loading messages
		LOG_INFO("Sim", "Loading, please wait...");

		// Create scene
		CreateScene();

		// Print instructions
		LOG_INFO("Sim", "Solar System Animation");
		LOG_INFO("Sim", "Instructions:");
		LOG_INFO("Sim", "- Press 'c' to stop camera rotation");
		LOG_INFO("Sim", "- Press left/right to navigate through the planets/stars");
		LOG_INFO("Sim", "- Use the mouse wheel to zoom in/out of the planet/star");
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
	Delete(g_AnimationPlayer);

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