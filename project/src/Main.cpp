#include "GraphicsManager.h"
#include "Object.h"

#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// TODO: ...
GraphicsManager *g_GraphicsManager;
Shader *g_Shader;

Object *g_CubeObject1;
Object *g_CubeObject2;


int windowId = 0;

// projection matrix - perspective projection
glm::mat4 projectionMatrix;

// view matrix - orient everything around our preferred view
glm::mat4 viewMatrix;

GLuint vertexBuffer;
GLuint indexBuffer;
GLenum positionBufferId;

float xRotation = 0.0f;
float yRotation = 0.0f;
float zRotation = 0.0f;

float yRotationSpeed = 0.1f;

float minArmRotation = 45.0f;
float maxArmRotation = 160.0f;
float upperArmRotation = 45.0f;
float upperArmRotationSpeed = 1.0f;

void drawFinger(glm::vec3 basePosition);
void drawCube(glm::mat4 modelMatrix, glm::vec4 colour);

static const GLfloat vertexPositionData[] = {
	-1.0f, -1.0f, 1.0f,  // front
	1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  // back
	1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f
};
static const GLushort indexData[] = {
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
int numVertices = 36;

static void createGeometry() 
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionData), vertexPositionData, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
}

static void update() 
{
	int milliseconds = glutGet(GLUT_ELAPSED_TIME);

	auto transform = g_CubeObject1->GetTransform();
	auto pos = transform->GetPosition();
	pos += transform->Right() / 100.0f;
	transform->SetPosition(pos);

	// rotate the entire model, to that we can examine it
	yRotation += yRotationSpeed;

	glutPostRedisplay();
}

static void render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// turn on depth buffering
	glEnable(GL_DEPTH_TEST);

	// activate our shader program
	//glUseProgram(programId);
	g_Shader->Apply();

	// colours
	glm::vec4 red(1.0, 0.0, 0.0, 1.0);
	glm::vec4 green(0.0, 1.0, 0.0, 1.0);

	/*glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.4f, 2.4f, 0.5f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -1.5f, 0.0f));*/
	drawCube(g_CubeObject1->GetTransform()->GetMatrix(), red);
	drawCube(g_CubeObject2->GetTransform()->GetMatrix(), green);

	// TODO:  Draw the fingers (and thumb)

	// make the draw buffer to display buffer (i.e. display what we have drawn)
	glutSwapBuffers();
}

void drawFinger(glm::vec3 basePosition) 
{
	// colours
	glm::vec4 colour1(0.8, 0.0, 0.5, 1.0);
	glm::vec4 colour2(0.0, 0.0, 0.8, 1.0);
	glm::vec4 colour3(0.0, 0.5, 0.5, 1.0);

	// TODO:  Draw three transformed cubes to make the finger
}

void drawCube(glm::mat4 modelMatrix, glm::vec4 colour) 
{
	// model-view-projection matrix
	glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
	g_Shader->GetVariable("u_MVP")->SetVecMatrixFloat4(1, false, &mvp[0][0]);

	// Cube colour
	g_Shader->GetVariable("u_colour")->SetVecFloat4(1, &colour[0]);

	// enable the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// configure the attribute array (the layout of the vertex buffer)
	glVertexAttribPointer(positionBufferId, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, NULL);
	glEnableVertexAttribArray(positionBufferId);

	// draw the triangle strip
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_SHORT, NULL);

	// disable the attribute array
	glDisableVertexAttribArray(positionBufferId);
}

static void reshape(int width, int height) 
{
	const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

	// if using perpsective projection, update projection matrix
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
	if (key == 'r') 
	{
		if (yRotationSpeed > 0.0) 
		{
			yRotationSpeed = 0.0;
		}
		else 
		{
			yRotationSpeed = 0.1;
		}
	}
	else if (key == 27) 
	{
		glutDestroyWindow(windowId);
		exit(0);
	}
	std::cout << "Key pressed: " << key << std::endl;
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowSize(800, 600);
	windowId = glutCreateWindow("CSCI4110U Final Project");

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

	createGeometry();

	// create the view matrix (position and orient the camera)
	viewMatrix = glm::lookAt(
		glm::vec3(0, 0, 25), // eye/camera location
		glm::vec3(0, 0, 0),    // where to look
		glm::vec3(0, 1, 0)     // up
	);

	// Test:
	g_GraphicsManager = new GraphicsManager("data/shaders");
	g_Shader = g_GraphicsManager->GetShader("main");

	// Test: 
	g_CubeObject1 = new Object("Cube1");
	g_CubeObject1->GetTransform()->SetScale({ 1.0f, 1.0f, 1.0f });
	g_CubeObject1->GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
	g_CubeObject1->GetTransform()->SetRotation(glm::quat({ 0.0f, glm::radians(-45.0f), 0.0f }));

	g_CubeObject2 = new Object("Cube2");
	g_CubeObject1->AddChild(g_CubeObject2);
	g_CubeObject2->GetTransform()->SetScale({ 1.0f, 1.0f, 1.0f });
	g_CubeObject2->GetTransform()->SetPosition({ 0.0f, 3.0f, 0.0f });
	g_CubeObject2->GetTransform()->SetRotation(glm::quat({ 0.0f,  glm::radians(45.0f), 0.0f }));

	glutMainLoop();

	// TODO: Shutdown

	return 0;
}