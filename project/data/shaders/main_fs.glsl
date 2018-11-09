#version 330 core

out vec4 FragColor;

in vec3 Color;
in vec2 TexCoords;

void main()
{
	FragColor = vec4(Color.x, Color.y, Color.z, 1.0);
}