#include "Memory.h"
#include "Log.h"
#include "Window.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#define WINDOW_TITLE "CSCI4110U Final Project"
#define WIDTH 1024
#define HEIGHT 600
#define FULLSCREEN false
#define WIREFRAME false // TODO: Testing

// Vars
Window *g_Window;
float g_LastUpdateTime = 0;
float g_LastRenderTime = 0;

// Forward declarations
extern bool Project_Initialize(Window *window);
extern bool Project_Shutdown();
extern void Project_Update(float time, float deltaTime);
extern void Project_Render(float time, float deltaTime);

static void Update()
{
	const auto time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	const auto deltaTime = time - g_LastUpdateTime;

	Project_Update(time, deltaTime);

	glutPostRedisplay();

	g_LastUpdateTime = time;
}

static void WindowClose()
{
	// Exit loop and proceed with shutdown
	glutLeaveMainLoop();
}

static void WindowRender()
{
	const auto time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	const auto deltaTime = time - g_LastRenderTime;

	Project_Render(time, deltaTime);
	
	// Make the draw buffer to display buffer
	g_Window->SwapBuffers();

	g_LastRenderTime = time;
}

#ifdef DEBUG
static void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar *message, const void *userParam)
{
	// Ignore notifications
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

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
	g_Window = New<Window>(WINDOW_TITLE, width, height, flags);
	g_Window->OnClose.Add(WindowClose);
	g_Window->OnRender.Add(WindowRender);

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
			
	// Turn on depth buffering
	glEnable(GL_DEPTH_TEST);

	// Try initializing
	if (!Project_Initialize(g_Window))
	{
		// Exit with error
		return 1;
	}

	// Run main loop
	glutMainLoop();

	// Try shutting down
	if (!Project_Shutdown())
	{
		// Exit with error
		return 1;
	}

	try
	{
		// Deallocate window
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

	// Exit successfully
	return 0;
}