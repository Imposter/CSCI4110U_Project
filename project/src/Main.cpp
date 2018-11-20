#include "Memory.h"
#include "Log.h"
#include "Window.h"
#include "GraphicsManager.h"
#include "Object.h"
#include "ModelManager.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define WIDTH 1024
#define HEIGHT 600
#define FULLSCREEN false
#define WIREFRAME false

// Constants
const glm::vec3 Up(0.0f, 1.0f, 0.0f);
const glm::vec3 LookTarget(0.0f, 0.0f, 0.0f);
const glm::vec3 EyePosition(10.0, 10.0f, 10.0f);

Window *g_Window;
unsigned int g_Width;
unsigned int g_Height;

GraphicsManager *g_GraphicsManager;
ModelManager *g_ModelManager;

// Program variables
float g_LastTime;
glm::mat4 g_ViewMatrix;
glm::mat4 g_ProjectionMatrix;

// App
Shader *g_Shader;
Model *g_Model;

// Test
bool g_DirectionalLightEnabled; // TODO: Make light classes and start on this
glm::vec3 g_DirectionalLightDirection(0.0f);
glm::vec3 g_DirectionalLightColor(0.80f, 0.80f, 0.80f);
glm::mat4 g_EarthMatrix;
glm::mat4 g_MoonMatrix;
float g_MoonRotation = 0.0f;

static void Update()
{
	const auto time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	const auto deltaTime = time - g_LastTime;

	// Set earth matrix
	g_EarthMatrix = glm::mat4(1.0f);
	g_EarthMatrix = glm::scale(g_EarthMatrix, glm::vec3(0.005f));//glm::vec3(1.5f));

	// Set moon matrix
	g_MoonRotation += deltaTime / 2.0f;
	g_MoonMatrix = glm::mat4(1.0f);
	g_MoonMatrix = glm::scale(g_MoonMatrix, glm::vec3(0.0001f));//glm::vec3(0.25f));
	glm::vec3 moonPosition(cos(g_MoonRotation) * 2.0f, sin(g_MoonRotation) * 2.0f, 2.0f);
	g_MoonMatrix = glm::translate(g_MoonMatrix, moonPosition);

	// Use shader
	g_Shader->Use();

	// Great site to learn from (avoid Cg): https://www.3dgep.com/texturing-and-lighting-with-opengl-and-glsl/

	// Update view matrix and shit
	g_ViewMatrix = glm::mat4(1.0f);
	const auto aspectRatio = static_cast<float>(g_Width) / static_cast<float>(g_Height);
	g_ProjectionMatrix = glm::ortho(-10.0f * aspectRatio, 10.0f * aspectRatio, -10.0f, 10.0f, -100.0f, 100.0f);

	// Set view position
	g_Shader->GetVariable("u_ViewPosition")->SetVec3(EyePosition); // -- this should not matter in the slightest -- so why do we use it?

	// Set directional light information
	g_DirectionalLightDirection = glm::normalize(moonPosition);
	g_Shader->GetVariable("u_DirectionalLight.Enabled")->SetBool(g_DirectionalLightEnabled);
	g_Shader->GetVariable("u_DirectionalLight.Direction")->SetVec3(g_DirectionalLightDirection);
	g_Shader->GetVariable("u_DirectionalLight.Ambient")->SetVec3(g_DirectionalLightColor * 0.25f);
	g_Shader->GetVariable("u_DirectionalLight.Diffuse")->SetVec3(g_DirectionalLightColor);
	g_Shader->GetVariable("u_DirectionalLight.Specular")->SetVec3(glm::vec3(1.0f, 1.0f, 1.0f));
	g_Shader->GetVariable("u_DirectionalLight.Intensity")->SetFloat(1.0f);

	glutPostRedisplay();

	g_LastTime = time;
}

static void WindowClose()
{
	// Exit loop and proceed with shutdown
	glutLeaveMainLoop();
}

static void WindowRender()
{
	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Turn on depth buffering
	glEnable(GL_DEPTH_TEST);

	// Update shader
	g_Shader->Use();
	g_Shader->GetVariable("u_View")->SetMat4(g_ViewMatrix); // TODO/NOTE: These should automatically be set for any shader being used to render something
	g_Shader->GetVariable("u_Projection")->SetMat4(g_ProjectionMatrix);

	// Render Earth
	g_Shader->GetVariable("u_Model")->SetMat4(g_EarthMatrix);
	g_Model->Render(nullptr);

	// Render Moon
	g_Shader->GetVariable("u_Model")->SetMat4(g_MoonMatrix);
	g_Model->Render(nullptr);

	// Make the draw buffer to display buffer
	g_Window->SwapBuffers();
}

static void WindowResize(ResizeEventArgs &args)
{
	// Divide by zero
	if (args.Width == 0 || args.Height == 0)
		return;

	const auto aspectRatio = static_cast<float>(args.Width) / static_cast<float>(args.Height);
	g_ProjectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

	// If using perpsective projection, update projection matrix
	glViewport(0, 0, args.Width, args.Height);

	// Store values
	g_Width = args.Width;
	g_Height = args.Height;
}

static void WindowKeyDown(KeyEventArgs &args)
{
	if (args.Value == kKey_Escape)
	{
		// Close window and proceed with shutdown
		g_Window->Close();
	}
}

static void WindowKeyPress(KeyPressEventArgs &args)
{
	if (args.Char == 'd')
	{
		// Toggle directional light
		g_DirectionalLightEnabled = !g_DirectionalLightEnabled;
	}
}

#ifdef DEBUG
static void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar *message, const void *userParam)
{
	const char *sourceStr;
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		sourceStr = "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		sourceStr = "Window";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		sourceStr = "Shader";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		sourceStr = "3rd Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		sourceStr = "Application";
		break;
	default:
		sourceStr = "Other";
		break;
	}

	const char *typeStr;
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		typeStr = "Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		typeStr = "Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		typeStr = "Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		typeStr = "Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		typeStr = "Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		typeStr = "Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		typeStr = "Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		typeStr = "Pop Group";
		break;
	default:
		typeStr = "Other";
		break;
	}

	const char *severityStr;
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		severityStr = "High";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		severityStr = "Medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		severityStr = "Low";
		break;
	default:
		severityStr = "Notification";
	}

	LOG_TRACE("OpenGL", "[%s][%s] %s: %s", sourceStr, severityStr, typeStr, message);
}
#endif

int main(int argc, char **argv)
{
	// Initialize logging
	LogOpen("Project.log",
#ifndef DEBUG
		kLogType_Error
#else
		kLogType_Trace
#endif
	);

	// Initialize memory subsystem
	MemoryInitialize();

	// Initialize GLUT
	glutInit(&argc, argv);

	// Set update function
	glutIdleFunc(&Update);

	// Set glut window shutdown behaviour
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	// Window config
	const auto width = FULLSCREEN ? glutGet(GLUT_SCREEN_WIDTH) : WIDTH;
	const auto height = FULLSCREEN ? glutGet(GLUT_SCREEN_HEIGHT) : HEIGHT;
	auto flags = 0;
	flags |= Window::kFlag_DoubleBuffered;
	if (FULLSCREEN) flags |= Window::kFlag_Fullscreen;

	// Create window
	g_Window = New<Window>("CSCI4110U Final Project", width, height, flags);
	g_Window->OnClose.Add(WindowClose);
	g_Window->OnRender.Add(WindowRender);
	g_Window->OnResize.Add(WindowResize);
	g_Window->OnKeyDown.Add(WindowKeyDown);
	g_Window->OnKeyPress.Add(WindowKeyPress);

	// Initialize OpenGL
	glewInit();

	// Check for minimum supported API
	if (!GLEW_VERSION_4_1)
	{
		LOG_ERROR("Main", "OpenGL 4.1 not available");

		// No cleanup
		return 1;
	}

	LOG_INFO("Main", "Using GLEW %s", glewGetString(GLEW_VERSION));
	LOG_INFO("Main", "Using OpenGL %s", glGetString(GL_VERSION));

#ifdef DEBUG
	// Set debug callback
	glDebugMessageCallback(DebugCallback, nullptr);

	// Enable debug mode
	glEnable(GL_DEBUG_OUTPUT);
#endif

#if WIREFRAME
	// Wireframe mode
	glPolygonMode(GL_FRONT, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
#endif

	// Create the view matrix (position and orient the camera)
	g_ViewMatrix = glm::lookAt(EyePosition, LookTarget, Up);

	// Create graphics manager
	g_GraphicsManager = New<GraphicsManager>("data");

	try
	{
		// Create shader
		g_Shader = g_GraphicsManager->GetShader("Diffuse");

		// Create model manager
		g_ModelManager = New<ModelManager>("data/models", g_GraphicsManager);

		// Load model
		g_Model = g_ModelManager->GetModel("earth");
	}
	catch (ShaderCompileException &ex)
	{
		LOG_TRACE("Main", ex.what());

		// No cleanup
		return 1;
	}
	catch (ModelLoadException &ex)
	{
		LOG_TRACE("Main", ex.what());

		// No cleanup
		return 1;
	}

	// Run main loop
	glutMainLoop();

	try
	{
		// Shutdown
		Delete(g_ModelManager);
		Delete(g_GraphicsManager);

		// Close window
		Delete(g_Window);

		// Shutdown memory subsystem
		MemoryShutdown();

		// Close log
		LogClose();
	}
	catch (MemoryLeakException &ex)
	{
		// Log
		LOG_TRACE("Main", ex.what());

		// Find out what leaked
		MemoryDebug();

		// Exit with error
		return 1;
	}

	return 0;
}