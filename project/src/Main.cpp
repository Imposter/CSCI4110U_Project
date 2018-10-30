#include "Memory.h"
#include "Log.h"
#include "GraphicsManager.h"
#include "Object.h"
#include "TextureUtil.h"
#include "Texture.h"

#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Program variables
int g_WindowID = 0;
glm::mat4 g_ViewMatrix;
glm::mat4 g_ProjectionMatrix;

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

static void update()
{
	const auto milliseconds = glutGet(GLUT_ELAPSED_TIME);



	glutPostRedisplay();
}

static void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Turn on depth buffering
	glEnable(GL_DEPTH_TEST);

	// Activate our shader program
	g_Shader->Apply();

	// TODO: Draw our texture onto a square

	// Make the draw buffer to display buffer (i.e. display what we have drawn)
	glutSwapBuffers();
}

static void reshape(int width, int height)
{
	// Divide by zero
	if (width == 0 || height == 0)
		return;

	const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	g_ProjectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

	// If using perpsective projection, update projection matrix
	glViewport(0, 0, width, height);
}

static void drag(int x, int y)
{
}

static void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
	}
}

static void keyboard(unsigned char key, int x, int y)
{
	if (key == VK_ESCAPE)
	{
		glutLeaveMainLoop();
	}

	std::cout << "Key pressed: " << key << std::endl;
}

int main(int argc, char **argv)
{
	// Initialize memory subsystem
	Memory::Initialize();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowSize(800, 600);
	g_WindowID = glutCreateWindow("CSCI4110U Final Project");

	glutIdleFunc(&update);
	glutDisplayFunc(&render);
	glutReshapeFunc(&reshape);
	glutMotionFunc(&drag);
	glutMouseFunc(&mouse);
	glutKeyboardFunc(&keyboard);

	glewInit();
	if (!GLEW_VERSION_2_0)
	{
		std::cerr << "OpenGL 2.0 not available" << std::endl;
		return 1;
	}
	std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;

	// Create the view matrix (position and orient the camera)
	g_ViewMatrix = glm::lookAt(
		glm::vec3(0, 0, 25),	// eye/camera location
		glm::vec3(0, 0, 0),		// where to look
		glm::vec3(0, 1, 0)		// up
	);

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
	g_Texture = LoadTextureFromFile("data/textures/brick.jpg");

	// TODO/NOTE: For how Vertex classes (arrays/buffers should be done -- see: https://www.opengl.org/discussion_boards/showthread.php/198946-OpenGL-VAO-VBO-EBO-explained)
	// TODO: Or we can do that in the mesh class like in Assignment 3 from last year...?
	{
		// Generate vertex arrays
		//glGenVertexArrays()
	}

	// Run main loop
	glutMainLoop();

	// Close window
	glutDestroyWindow(g_WindowID);

	try
	{
		// Delete texture
		Delete(g_Texture);

		// Shutdown
		Delete(g_Root);
		Delete(g_GraphicsManager);

		// Shutdown memory subsystem
		Memory::Shutdown();
	}
	catch (MemoryLeakException &ex)
	{
		// Log
		LOG_TRACE("Main", ex.what());

		// Find out what leaked
		Memory::Debug();

		// Exit with error
		return 1;
	}

	return 0;
}