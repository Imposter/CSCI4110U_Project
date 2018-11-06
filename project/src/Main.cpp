#include "Memory.h"
#include "Log.h"
#include "Window.h"
#include "GraphicsManager.h"
#include "Object.h"
#include "Mesh.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define WIDTH 1024
#define HEIGHT 600
#define FULLSCREEN false

// Constants
const glm::vec3 Zero(0.0f, 0.0f, 0.0f);
const glm::vec3 Up(0.0f, 1.0f, 0.0f);

// Program variables
glm::mat4 g_ViewMatrix;
glm::mat4 g_ProjectionMatrix;

Window *g_Window;

GraphicsManager *g_GraphicsManager;
Shader *g_Shader;

Object *g_Root;
Object *g_Square;

Texture *g_Texture;

Mesh *g_Mesh;

// Square
float g_SquareVertices[] = {
	// Positions          // Colors           // Texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top left 
};

unsigned int g_SquareIndices[] = {
	0, 1, 3, // First triangle
	1, 2, 3  // Second triangle
};

static void Update()
{
	const auto milliseconds = glutGet(GLUT_ELAPSED_TIME);

	// TODO: Transform updates

	glutPostRedisplay();
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

	// Bind texture
	g_Texture->Bind();

	// Activate our shader program
	g_Shader->Use();

	// Render
	g_Mesh->Render(nullptr);

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
}

static void WindowKeyDown(KeyEventArgs &args)
{
	if (args.Value == kKey_Escape)
	{
		// Close window and proceed with shutdown
		g_Window->Close();
	}
}

#ifdef DEBUG
void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
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

	// Initialize OpenGL
	glewInit();
	if (!GLEW_VERSION_3_0)
	{
		LOG_ERROR("Main", "OpenGL 3.0 not available");

		// Delete window
		Delete(g_Window);

		// Shutdown memory subsystem
		MemoryShutdown();

		// Close log
		LogClose();

		return 1;
	}

	LOG_INFO("Main", "Using GLEW %s", glewGetString(GLEW_VERSION));
	LOG_INFO("Main", "Using OpenGL %s", glGetString(GL_VERSION));

#ifdef DEBUG
	// Set debug callback
	glDebugMessageCallback(DebugCallback, nullptr);
#endif

	// Create the view matrix (position and orient the camera)
	g_ViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 25.0f), Zero, Up);

	// Create root object
	g_Root = New<Object>("Root");

	// Create graphics manager
	g_GraphicsManager = New<GraphicsManager>("data");
	g_Shader = g_GraphicsManager->GetShader("main");

	// Create objects
	g_Square = g_Root->CreateChild<Object>("Square");
	g_Square->GetTransform()->SetScale({ 1.0f, 1.0f, 1.0f });
	g_Square->GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
	g_Square->GetTransform()->SetRotation(glm::quat({ 0.0f, 0.0f, 0.0f }));

	// Create texture
	g_Texture = g_GraphicsManager->GetTexture("container");

	// Generate mesh
	{
		std::vector<MeshVertex> vertices;
		vertices.resize(sizeof(g_SquareVertices) / sizeof(MeshVertex));
		memcpy(vertices.data(), g_SquareVertices, sizeof(g_SquareVertices));

		std::vector<unsigned int> indices;
		indices.resize(sizeof(g_SquareIndices) / sizeof(float));
		memcpy(indices.data(), g_SquareIndices, sizeof(g_SquareIndices));

		g_Mesh = New<Mesh>("SquareMesh", vertices, indices);
		g_Mesh->Compile();
	}

	// Run main loop
	glutMainLoop();

	try
	{
		// Delete mesh
		Delete(g_Mesh);

		// Shutdown
		Delete(g_Root);
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

		// Close log
		LogClose();

		// Exit with error
		return 1;
	}

	return 0;
}