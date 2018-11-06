#include "Memory.h"
#include "Log.h"
#include "Window.h"
#include "GraphicsManager.h"
#include "Object.h"
#include "TextureUtil.h"
#include "Texture.h"
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

// Test
//Mesh *g_Mesh;

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

VertexArray *g_SquareVAO;
Buffer *g_SquareVBO;
Buffer *g_SquareEBO;

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
	//g_Mesh->Render(nullptr); // TODO/NOTE: Partly works

	g_SquareVAO->Bind();
	glDrawElements(GL_TRIANGLES, sizeof(g_SquareIndices) / sizeof(float), GL_UNSIGNED_INT, 0);

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
	g_GraphicsManager = New<GraphicsManager>("data/shaders");
	g_Shader = g_GraphicsManager->GetShader("main");

	// Create objects
	g_Square = g_Root->CreateChild<Object>("Square");
	g_Square->GetTransform()->SetScale({ 1.0f, 1.0f, 1.0f });
	g_Square->GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
	g_Square->GetTransform()->SetRotation(glm::quat({ 0.0f, 0.0f, 0.0f }));

	// Create texture
	g_Texture = LoadTextureFromFile("data/textures/joker.png", Texture::kFormat_RGBA);

	// Generate mesh
	{
		std::vector<MeshVertex> vertices;
		for (size_t i = 0; i < sizeof(g_SquareVertices) / sizeof(MeshVertex); i++)
		{
			MeshVertex v;
			v.Position = glm::vec3(g_SquareVertices[i + 0], g_SquareVertices[i + 1], g_SquareVertices[i + 2]);
			v.Normal = glm::vec3(g_SquareVertices[i + 3], g_SquareVertices[i + 4], g_SquareVertices[i + 5]); // Color
			v.TexCoords = glm::vec2(g_SquareVertices[i + 6], g_SquareVertices[i + 7]);

			vertices.push_back(v);
		}

		LOG_TRACE("Main", "sizeof(MeshVertex)=%d", sizeof(MeshVertex));

		std::vector<unsigned int> indices;
		for (size_t i = 0; i < sizeof(g_SquareIndices) / sizeof(float); i++)
		{
			indices.push_back(g_SquareIndices[i]);
		}

		//g_Mesh = New<Mesh>("SquareMesh", vertices, indices, std::vector<Texture *>());// TODO/NOTE: Partly works...
		//g_Mesh->Compile();
	}

	{
		g_SquareVAO = New<VertexArray>(MeshVertex::GetAttributes());

		g_SquareVAO->Bind();

		g_SquareVBO = New<Buffer>(Buffer::kTarget_ArrayBuffer, sizeof(g_SquareVertices), g_SquareVertices);
		g_SquareEBO = New<Buffer>(Buffer::kTarget_ElementArrayBuffer, sizeof(g_SquareIndices), g_SquareIndices);

		g_SquareVAO->Apply();
	}

	// Run main loop
	glutMainLoop();

	try
	{
		// Delete buffers
		Delete(g_SquareVAO);
		Delete(g_SquareEBO);
		Delete(g_SquareVBO);

		// Delete mesh
		//Delete(g_Mesh);

		// Delete texture
		DestroyTexture(g_Texture);

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