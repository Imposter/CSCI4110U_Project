#version 410 core

// Type definitions
struct Material
{
	vec3 Ambient; // Unused
	vec3 Diffuse;
	vec3 Specular; // Unused
	
	sampler2D TextureAmbient; // Unused
	bool TextureAmbientEnabled; // Unused
	sampler2D TextureDiffuse;
	bool TextureDiffuseEnabled;
	sampler2D TextureSpecular; // Unused
	bool TextureSpecularEnabled; // Unused

	float Shininess; // Unused
};

// Obj
uniform Material u_Material;

// Input vars
in vec2 TexCoords;

// Output vars
out vec4 FragColor;

void main()
{
	if (u_Material.TextureDiffuseEnabled)
		FragColor = vec4(texture(u_Material.TextureDiffuse, TexCoords).rgb * u_Material.Diffuse, 1.0f);
	else FragColor = vec4(u_Material.Diffuse, 1.0f);
}