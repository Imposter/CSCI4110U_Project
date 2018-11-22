#include "../Window.h"
#include "../Log.h"
#include "../GraphicsManager.h"
#include "../ModelManager.h"
#include "../Camera.h"
#include "../Object.h"
#include "Util.h"
#include "Star.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Constants
const float CameraFOV = 50.0f;
const float CameraNearClip = 0.01f;
const float CameraFarClip = 1000000.0f;
const glm::vec4 CameraClearColor(0.0f, 0.0f, 0.0f, 1.0f);
const float CameraClearDepth = 1.0f;

const glm::vec3 LookTarget(0.0f, 0.0f, 0.0f);
const glm::vec3 EyePosition(0.0f, 0.0f, 30.0f); // X, Y, -Z (where X is left/right, Y is up/down, Z is in/out)

const int StarCount = 500;
const float StarInnerRadius = 10.0f;
const float StarOuterRadius = 50.0f;
const float StarMinSize = 0.075f;
const float StarMaxSize = 0.10f;

const glm::vec3 ShipStartPosition(0.0f, -10.0f, 0.0f);

// Vars
unsigned int g_Width;
unsigned int g_Height;

Window *g_RootWindow;
GraphicsManager *g_GraphicsManager;
ModelManager *g_ModelManager;

Camera *g_Camera;
Object *g_RootObject;
Node *g_RootNode;

Shader *g_FlatShader;

Model *g_ShipModel;

void CreateScene()
{
	// Generate star field
	GenerateStars(StarCount, StarInnerRadius, StarOuterRadius, StarMinSize, StarMaxSize, g_FlatShader, g_RootObject, g_RootNode);

	// Load model
	g_ShipModel = g_ModelManager->LoadModel("Ship1");
	g_RootNode->AddChild(g_ShipModel);

	// Set ship transform
	const auto shipTransform = g_ShipModel->GetTransform();
	shipTransform->SetScale(glm::vec3(1.0f));
	shipTransform->SetPosition(ShipStartPosition);
}

void Project_Update(float time, float deltaTime)
{
	//// TEST: Slowly move cam
	//const auto tCam = g_Camera->GetTransform(); // TODO/NOTE: Why are the axis busted on the cam?
	////transform->OffsetRotation(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(1.0f));
	////tCam->OffsetPosition(tCam->Forward() * 0.01f);

	const auto tModel = g_ShipModel->GetTransform();
	tModel->OffsetPosition(tModel->Right() * 0.1f);
	////tModel->OffsetRotation(tModel->Forward(), glm::radians(5.0f));

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

bool Project_Initialize(Window *window)
{
	// Store window
	g_RootWindow = window;

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
		g_Camera->SetClearMode(kCameraClearMode_Both);
		g_Camera->SetClearColor(CameraClearColor);
		g_Camera->SetClearDepth(CameraClearDepth);

		//g_Camera->GetTransform()->SetPosition(g_Camera->GetTransform()->Up() * 5.0f); // Move right
		g_Camera->GetTransform()->SetPosition(EyePosition);
		g_Camera->GetTransform()->LookAt(LookTarget);

		// Create root object
		g_RootObject = New<Object>("Root");

		// Create root node
		g_RootNode = New<Node>("Root");

		// Get flat shader
		g_FlatShader = g_GraphicsManager->GetShader("Flat");
		g_FlatShader->Use(); // We shouldn't need to use here, but there's errors

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
	// Shutdown
	Delete(g_RootNode);
	Delete(g_RootObject);
	Delete(g_Camera);
	Delete(g_ModelManager);
	Delete(g_GraphicsManager);

	return true;
}