#version 410 core

// Set precisions
precision highp float;

// Type definitions
struct Material
{
	vec3 Diffuse;

	bool TextureDiffuseEnabled;
	sampler2D TextureDiffuse;
};

// Obj
uniform Material u_Material;

// Input vars
in vec2 TexCoords;

// Output vars
out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0f);
	if (u_Material.TextureDiffuseEnabled) 
	{
		FragColor = vec4(texture(u_Material.TextureDiffuse, TexCoords).rgb * u_Material.Diffuse, 1.0f);
	}
}