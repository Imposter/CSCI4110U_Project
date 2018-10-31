#include "Memory.h"
#include "Log.h"
#include "Window.h"
#include "GraphicsManager.h"
#include "Object.h"
#include "TextureUtil.h"
#include "Texture.h"

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

GLuint g_SquareVAO; // Vertex array -- stores vertex format (position, color, tex coord)
GLuint g_SquareVBO; // Vertex buffer
GLuint g_SquareEBO; // Index buffer

static void Update()
{
	const auto milliseconds = glutGet(GLUT_ELAPSED_TIME);

	// TODO: Transform updates

	glutPostRedisplay();
}

static void WindowRender(EventArgs &args)
{
	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Turn on depth buffering
	glEnable(GL_DEPTH_TEST);

	// Bind texture
	g_Texture->Bind();

	// Activate our shader program
	g_Shader->Apply();

	// Render
	glBindVertexArray(g_SquareVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
		glutLeaveMainLoop();
	}
}

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

	// Window config
	const auto width = FULLSCREEN ? glutGet(GLUT_SCREEN_WIDTH) : WIDTH;
	const auto height = FULLSCREEN ? glutGet(GLUT_SCREEN_HEIGHT) : HEIGHT;
	auto flags = 0;
	flags |= Window::kFlag_DoubleBuffered;
	if (FULLSCREEN) flags |= Window::kFlag_Fullscreen;

	// Create window
	g_Window = New<Window>("CSCI4110U Final Project", width, height, flags);

	g_Window->OnRender.Add(WindowRender);
	g_Window->OnResize.Add(WindowResize);
	g_Window->OnKeyDown.Add(WindowKeyDown);

	// Initialize OpenGL
	glewInit();
	if (!GLEW_VERSION_2_0)
	{
		LOG_ERROR("Main", "OpenGL 2.0 not available");

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

	// TODO/NOTE: For how Vertex classes (arrays/buffers should be done -- see: https://www.opengl.org/discussion_boards/showthread.php/198946-OpenGL-VAO-VBO-EBO-explained)
	// TODO: Or we can do that in the mesh class like in Assignment 3 from last year...?
	{
		// Generate vertex array
		glGenVertexArrays(1, &g_SquareVAO);

		// Generate buffers
		glGenBuffers(1, &g_SquareVBO);
		glGenBuffers(1, &g_SquareEBO);

		// Bind vertex array
		glBindVertexArray(g_SquareVAO);

		// Bind buffers
		glBindBuffer(GL_ARRAY_BUFFER, g_SquareVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_SquareVertices), g_SquareVertices, GL_STATIC_DRAW);
		
		// TODO/NOTE: We will not need this for meshes/interleaved VBOs, we might be able to make a basic buffer class
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_SquareEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_SquareIndices), g_SquareIndices, GL_STATIC_DRAW);

		// Set vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(0)); // Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float))); // Color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float))); // Texture coordinate
		glEnableVertexAttribArray(2);

		// TODO: We can store meta data with the meshes/models that we load and create vertex arrays with
		// TODO: their attributes then -- https://stackoverflow.com/questions/28775659/c-interface-for-managing-opengl-vertex-attributes
		// TODO: We should do that with Mesh formats -- with a meta data file like shaders
	}

	// Run main loop
	glutMainLoop();

	try
	{
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