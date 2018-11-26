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

void main()
{
	// Set vertex position
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Pos, 1.0f);
	
	// Set output vars
	TexCoords = a_TexCoords;
}