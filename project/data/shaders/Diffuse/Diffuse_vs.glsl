#version 330 core

// Attributes
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

// Input uniforms
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// Output vars
out vec3 Normal;
out vec2 TexCoords;
out vec3 WorldPos;

void main()
{
	// Set output vars
	Normal = a_Normal;
	TexCoords = a_TexCoords;

	// Calculate world position
	WorldPos = vec3(u_Model * vec4(a_Pos, 1.0f));

	// Set vertex position
	gl_Position = u_Projection * u_View * vec4(WorldPos, 1.0f);
}