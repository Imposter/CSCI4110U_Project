#version 330 core

#define POINT_LIGHTS_MAX 100

// Type definitions
struct Material
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	
	sampler2D TextureAmbient;
	bool TextureAmbientEnabled;
	sampler2D TextureDiffuse;
	bool TextureDiffuseEnabled;
	sampler2D TextureSpecular;
	bool TextureSpecularEnabled;

	float Shininess;
};

struct DirectionalLight
{
	bool Enabled;
	vec3 Direction;
	float Intensity;

	vec3 Color;
};

struct PointLight
{
	bool Enabled;
	vec3 Position;
	vec3 Direction;

	vec3 Color;
};

// Lights
uniform Material u_Material;
uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLights[POINT_LIGHTS_MAX];
uniform vec3 u_ViewPosition;

// Input vars
in vec3 Normal;
in vec2 TexCoords;
in vec3 WorldPos;

// Output vars
out vec4 FragColor;

vec3 ProcessPointLight(int index) 
{
	PointLight light = u_PointLights[index];

	// ...
	return vec3(0.0f);
}

vec3 ProcessDirectionalLight(vec3 normal, vec3 viewDir)
{
	if (!u_DirectionalLight.Enabled)
		return vec3(0.0f);

	// TODO: ...
	
}

void main()
{
	// Properties
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(u_ViewPosition - WorldPos);

	// Add lights
	vec3 color = ProcessDirectionalLight(normal, viewDir);
	for (int i = 0; i < POINT_LIGHTS_MAX; i++)
		color += ProcessPointLight(i);

	// test
	if (!u_DirectionalLight.Enabled)
	{
		color = vec3(1.0f, 0.0f, 0.0f);
	}

	// Set color
	FragColor = vec4(color, 1.0f);
}