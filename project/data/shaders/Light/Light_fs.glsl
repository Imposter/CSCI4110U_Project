#version 410 core

// Set precisions
precision highp float;

#define DIRECTIONAL_LIGHTS_MAX 1
#define POINT_LIGHTS_MAX 10

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
	vec3 Direction;
	float Intensity;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

struct PointLight
{
	vec3 Position;
	float Intensity;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	// TODO: Attenuation
};

// Uniforms
uniform Material u_Material;
uniform vec3 u_ViewPosition;
uniform int u_DirectionalLightCount;
uniform DirectionalLight u_DirectionalLights[DIRECTIONAL_LIGHTS_MAX];
uniform int u_PointLightCount;
uniform PointLight u_PointLights[POINT_LIGHTS_MAX];

// Input vars
in vec3 Normal;
in vec2 TexCoords;
in vec3 WorldPos;

// Output vars
out vec4 FragColor;

void ProcessDirectionalLight(in int index, in vec3 normal, in vec3 viewDirection, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
	vec3 lightDirection = normalize(-u_DirectionalLights[index].Direction);

	// Ambient
	ambient += u_DirectionalLights[index].Ambient * u_DirectionalLights[index].Intensity;

	// Diffuse
	float d = max(0.0f, dot(normal, lightDirection));
	diffuse += d * u_DirectionalLights[index].Diffuse * u_DirectionalLights[index].Intensity;

	// Specular
	vec3 reflectDirection = reflect(-lightDirection, normal);
    float s = pow(max(0.0f, dot(viewDirection, reflectDirection)), u_Material.Shininess);
	specular += s * u_DirectionalLights[index].Specular * u_DirectionalLights[index].Intensity;
}

void ProcessPointLight(in int index, in vec3 normal, in vec3 viewDirection, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) 
{
	vec3 lightDirection = normalize(u_PointLights[index].Position - WorldPos);

	// TODO: Attenuation

	// Ambient
	ambient += u_PointLights[index].Ambient * u_PointLights[index].Intensity;

	// Diffuse
	float d = max(0.0f, dot(normal, lightDirection));
	diffuse += d * u_PointLights[index].Diffuse * u_PointLights[index].Intensity;

	// Specular
	vec3 reflectDirection = reflect(-lightDirection, normal);
    float s = pow(max(0.0f, dot(viewDirection, reflectDirection)), u_Material.Shininess);
	specular += s * u_PointLights[index].Specular * u_PointLights[index].Intensity;
}

void main()
{
	// Normalize vectors
	vec3 normal = normalize(Normal);
	vec3 viewDirection = normalize(u_ViewPosition - WorldPos);
	
	// Calculate lighting
	vec3 ambient = vec3(0.0f);
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	
	// Apply directional lights
	for (int i = 0; i < u_DirectionalLightCount; i++)
		ProcessDirectionalLight(i, normal, viewDirection, ambient, diffuse, specular);

	// Apply point lights
	for (int i = 0; i < u_PointLightCount; i++)
		ProcessPointLight(i, normal, viewDirection, ambient, diffuse, specular);

	// Update colors with material properties
	if (u_Material.TextureAmbientEnabled)
		ambient *= texture(u_Material.TextureAmbient, TexCoords).rgb * u_Material.Ambient;
	else ambient *= u_Material.Ambient;
	if (u_Material.TextureDiffuseEnabled)
		diffuse *= texture(u_Material.TextureDiffuse, TexCoords).rgb * u_Material.Diffuse;
	else diffuse *= u_Material.Diffuse;
	if (u_Material.TextureSpecularEnabled)
		specular *= texture(u_Material.TextureSpecular, TexCoords).rgb * u_Material.Specular;
	else specular *= u_Material.Specular;

	// Set color
	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}