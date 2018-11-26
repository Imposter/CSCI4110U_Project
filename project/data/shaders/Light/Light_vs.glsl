#version 410 core

// Set precisions
precision highp float;

// Attributes
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

// Input uniforms
uniform mat4 u_Transform;
uniform mat4 u_View;
uniform mat4 u_Projection;

// Output vars
out vec3 Normal;
out vec2 TexCoords;
out vec3 WorldPos;

void main()
{
	// Set output vars
	Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	TexCoords = a_TexCoords;

	// Calculate world position
	WorldPos = vec3(u_Transform * vec4(a_Pos, 1.0f));

	// Set vertex position
	gl_Position = u_Projection * u_View * vec4(WorldPos, 1.0f);
}