#include "../Window.h"
#include "../Log.h"
#include "../GraphicsManager.h"
#include "../ModelManager.h"
#include "../LightManager.h"
#include "../Camera.h"
#include "../Object.h"
#include "UVSphere.h"
#include "Util.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SCALE_FACTOR 1.0f
#define SCALE_STAR_FACTOR 50.0f
#define SCALE_PLANET_FACTOR 1000.0f
#define SCALE_DISTANCE_FACTOR 1.0f
#define SOLAR_SYSTEM_RADIUS 600000000.0f // km
#define SCALE(x) ((x / SOLAR_SYSTEM_RADIUS) * SCALE_FACTOR)
#define SCALE_DISTANCE(x) (SCALE(x) * SCALE_DISTANCE_FACTOR)
#define SCALE_STAR(x) (SCALE(x) * SCALE_STAR_FACTOR)
#define SCALE_PLANET(x) (SCALE(x) * SCALE_PLANET_FACTOR)

// Working
/*#define SCALE_FACTOR_DISTANCE 10.0f
#define SCALE_FACTOR_OBJECT 50.0f
#define SOLAR_SYSTEM_RADIUS 600000000.0f
#define SCALE_DISTANCE(x) ((x / SOLAR_SYSTEM_RADIUS) * SCALE_FACTOR_DISTANCE)
#define SUN_RADIUS 695508.0f
#define OBJECT_MAX_RADIUS (SUN_RADIUS * 25000.0f) // km
#define SCALE_OBJECT(x) SCALE_DISTANCE(x)//#define SCALE_OBJECT(x) ((x / OBJECT_MAX_RADIUS) * SCALE_FACTOR_OBJECT)

#define SCALE_STAR_FACTOR 10.0f
#define SCALE_PLANET_FACTOR 100.0f
#define SCALE_STAR(x) (SCALE_OBJECT(x) * SCALE_STAR_FACTOR)
#define SCALE_PLANET(x) (SCALE_OBJECT(x) * SCALE_PLANET_FACTOR)*/

// Space
const float SkyboxScale = 5.0f;
const glm::vec3 SunPosition(0.0f, 0.0f, 0.0f);
const float SunScale = SCALE_STAR(695508.0f);
const float EarthDistance = SCALE_DISTANCE(149600000.0f); // From sun
const float EarthScale = SCALE_PLANET(6371.0f);
const float EarthMoveSpeed = 0.001;
const float EarthRotateSpeed = 2.0f;

// Constants
const float CameraFOV = 50.0f;
const float CameraNearClip = 0.00001f;
const float CameraFarClip = -1.0f; // Infinity
const glm::vec4 CameraClearColor(0.0f, 0.0f, 0.0f, 1.0f);
const float CameraClearDepth = 1.0f;
const glm::vec3 CameraDefaultPosition(EarthDistance * 1.0f, EarthDistance * 1.0f, -EarthDistance * 1.0f);

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

// Models
Model *g_SkyboxModel;
Model *g_SunModel;
Model *g_EarthModel;

// Camera
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
	const auto cameraMatrix = glm::lookAt(CameraDefaultPosition, SunPosition, glm::vec3(0.0f, 1.0f, 0.0f));
	cameraTransform->SetMatrix(cameraMatrix); // TODO: Somehow fix Transform::LookAt
	//cameraTransform->SetPosition(CameraDefaultPosition);
	//cameraTransform->LookAt(SunPosition);

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
	g_RootNode->AddChild(g_SkyboxModel);

	const auto skyboxTransform = g_SkyboxModel->GetTransform();
	skyboxTransform->SetScale(glm::vec3(SkyboxScale));

	// Create sun
	g_SunModel = g_ModelManager->LoadModel("Sun");
	g_RootNode->AddChild(g_SunModel);

	// Set sun transform
	const auto sunTransform = g_SunModel->GetTransform();
	sunTransform->SetScale(glm::vec3(SunScale));
	sunTransform->SetPosition(SunPosition);

	// Create earth
	g_EarthModel = g_ModelManager->LoadModel("Earth");
	g_RootNode->AddChild(g_EarthModel);

	// Set earth transform
	const auto earthTransform = g_EarthModel->GetTransform();
	earthTransform->SetScale(glm::vec3(EarthScale));

	// Set initial look target as sun
	g_LookTarget = sunTransform;
}

void Project_Update(float time, float deltaTime)
{
	// Get time in seconds
	const auto timeSeconds = time / 1000.0f;

	// Update earth
	{
		const auto earthTransform = g_EarthModel->GetTransform();
		
		glm::mat4 mat(1.0f);
		mat = glm::rotate(mat, timeSeconds * EarthRotateSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
		mat = glm::translate(mat, glm::vec3(EarthDistance * sin(timeSeconds * EarthMoveSpeed), 0.0f, EarthDistance * cos(timeSeconds * EarthMoveSpeed)));
		mat = glm::scale(mat, glm::vec3(EarthScale));

		earthTransform->SetMatrix(mat);

		//earthTransform->SetPosition(glm::vec3(EarthDistance * sin(timeSeconds * EarthMoveSpeed), 0.0f, EarthDistance * cos(timeSeconds * EarthMoveSpeed)));

		// Rotate earth on its own axis
		//earthTransform->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), timeSeconds * EarthRotateSpeed);	
	}

	// Set camera transform
	//const auto cameraTransform = g_Camera->GetTransform();
	//cameraTransform->LookAt(g_LookTarget->GetPosition());

	// Update objects
	g_RootObject->Update(time, deltaTime);

	// Update camera
	g_Camera->Update(deltaTime);
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

}

void Project_WindowMouseMove(MouseEventArgs &args)
{
	
}

void Project_WindowMouseDown(MouseEventArgs &args)
{

}

void Project_WindowMouseUp(MouseEventArgs &args)
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
	Delete(g_RootNode);
	Delete(g_RootObject);
	Delete(g_Camera);
	Delete(g_LightManager);
	Delete(g_ModelManager);
	Delete(g_GraphicsManager);

	return true;
}