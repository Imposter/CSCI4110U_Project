#version 330 core

// Attributes
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in vec2 a_TexCoords;

// Input uniforms
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// Output vars
out vec3 Normal;
out vec4 Color;
out vec2 TexCoords;
out vec3 WorldPos;

void main()
{
	// Set output vars
	Color = a_Color;
	TexCoords = a_TexCoords;

	// Calculate world position
	WorldPos = vec3(u_Model * vec4(a_Pos, 1.0f));

	// Calculate normal
	Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

	// Set vertex position
    gl_Position = u_Projection * u_View * vec4(WorldPos, 1.0f);
}