#version 330 core

#define POINT_LIGHTS_MAX 100

// Type definitions
struct Material
{
	vec3 Ambient; // Unused
	vec3 Diffuse;
	vec3 Specular; // Unused

	float Shininess; // Unused
};

struct DirectionalLight
{
	bool Enabled;
	vec3 Direction;
	float Intensity; // 0.0f-1.0f

	vec3 Color;
};

struct PointLight
{
	bool Enabled;
	vec3 Position;
	vec3 Direction;

	vec3 Color;
};

// Obj
uniform Material u_Material;
uniform vec3 u_ViewPosition;

// Lights
uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLights[POINT_LIGHTS_MAX];

// Input vars
in vec3 Normal;
in vec2 TexCoords;
in vec3 WorldPos;

// Output vars
out vec4 FragColor;

void ProcessPointLight(in int index, in vec3 normal, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) 
{
	PointLight light = u_PointLights[index];
	if (!light.Enabled)
		return;

	// ...
}

void ProcessDirectionalLight(in vec3 normal, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
	if (!u_DirectionalLight.Enabled)
		return;

	vec3 lightDir = -normalize(u_DirectionalLight.Direction);
	float NdotL = dot(Normal, -1.0f * lightDir);
	if (NdotL > 0.0f)
		diffuse = NdotL * u_DirectionalLight.Color * u_DirectionalLight.Intensity;
}

void main()
{
	// Properties
	vec3 normal = normalize(Normal);
	
	// Calculate lighting
	vec3 ambient = vec3(0.0f);
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	
	// Apply directional light
	ProcessDirectionalLight(normal, ambient, diffuse, specular);
	
	// Apply point light
	for (int i = 0; i < POINT_LIGHTS_MAX; i++)
		ProcessPointLight(i, normal, ambient, diffuse, specular);

	// Set color
	FragColor = vec4(u_Material.Diffuse * (ambient + diffuse + specular), 1.0f);
}