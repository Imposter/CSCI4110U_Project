#include "Memory.h"
#include "Log.h"
#include "Window.h"
#include "GraphicsManager.h"
#include "Object.h"

// test
#include "ModelUtil.h"

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
const glm::vec3 EyePosition(10.0f, 5.0f, 10.0f);

// Program variables
glm::mat4 g_ViewMatrix;
glm::mat4 g_ProjectionMatrix;
glm::mat4 g_ModelMatrix;

Window *g_Window;

GraphicsManager *g_GraphicsManager;
Object *g_Root;

// App
Shader *g_Shader;
Model *g_Model;
Mesh *g_Mesh;

static void Update()
{
	const auto milliseconds = glutGet(GLUT_ELAPSED_TIME);

	// Transform updates
	g_ModelMatrix = glm::mat4(1.0f);
	g_ModelMatrix = glm::scale(g_ModelMatrix, glm::vec3(1.0f));

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

	// Update shader
	g_Shader->Use();
	g_Shader->GetVariable("model")->SetVecMatrixFloat4(1, false, &g_ModelMatrix[0][0]);
	g_Shader->GetVariable("view")->SetVecMatrixFloat4(1, false, &g_ViewMatrix[0][0]);
	g_Shader->GetVariable("projection")->SetVecMatrixFloat4(1, false, &g_ProjectionMatrix[0][0]);

	// Render
	g_Mesh->Render(nullptr);//g_Model->Render(nullptr);

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

	// Enable debug mode
	glEnable(GL_DEBUG_OUTPUT);
#endif

	// Wireframe for mesh testing
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Create the view matrix (position and orient the camera)
	g_ViewMatrix = glm::lookAt(EyePosition, Zero, Up);

	// Create root object
	g_Root = New<Object>("Root");

	// Create graphics manager
	g_GraphicsManager = New<GraphicsManager>("data");

	// Create shader
	g_Shader = g_GraphicsManager->GetShader("main");

	// Load model
	//g_Model = LoadModelFromFile("data/models", "cube");

	{
		// Create test cube mesh
		std::vector<MeshVertex> vertices;

		// Front
		{
			{
				MeshVertex v;
				v.Position = glm::vec3(-1.0f, -1.0f, 1.0f);
				vertices.push_back(v);
			}
			{
				MeshVertex v;
				v.Position = glm::vec3(1.0f, -1.0f, 1.0f);
				vertices.push_back(v);
			}
			{
				MeshVertex v;
				v.Position = glm::vec3(-1.0f, 1.0f, 1.0f);
				vertices.push_back(v);
			}
			{
				MeshVertex v;
				v.Position = glm::vec3(1.0f, 1.0f, 1.0f);
				vertices.push_back(v);
			}
		}
		// Back
		{
			{
				MeshVertex v;
				v.Position = glm::vec3(-1.0f, -1.0f, -1.0f);
				vertices.push_back(v);
			}
			{
				MeshVertex v;
				v.Position = glm::vec3(1.0f, -1.0f, -1.0f);
				vertices.push_back(v);
			}
			{
				MeshVertex v;
				v.Position = glm::vec3(-1.0f, 1.0f, -1.0f);
				vertices.push_back(v);
			}
			{
				MeshVertex v;
				v.Position = glm::vec3(1.0f, 1.0f, -1.0f);
				vertices.push_back(v);
			}
		}

		// Set all the vertices to red
		for (auto &v : vertices)
			v.Colour = glm::vec3(1.0f, 0.0f, 0.0f);

		std::vector<unsigned int> indices = { 
			0, 1, 2,   // front
			3, 2, 1,
			5, 6, 7,   // back
			5, 4, 6,
			1, 7, 3,   // right
			1, 5, 7,
			4, 0, 2,   // left
			4, 2, 6,
			2, 7, 6,   // top
			2, 3, 7,
			0, 4, 5,   // bottom
			0, 5, 1 
		};

		g_Mesh = New<Mesh>("Test", vertices, indices);
	}

	// Run main loop
	glutMainLoop();

	try
	{
		// Delete model
		//DestroyModel(g_Model);

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