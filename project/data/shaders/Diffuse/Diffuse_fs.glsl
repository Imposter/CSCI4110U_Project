#version 330 core

// TODO: Split this texture for lights, and rename it to something other than Diffuse

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

	vec3 Ambient; // Not required
	vec3 Diffuse; // Required
	vec3 Specular; // Not required
};

struct PointLight
{
	bool Enabled;
	vec3 Position;
	vec3 Direction;

	vec3 Ambient; // Not required
	vec3 Diffuse; // Required
	vec3 Specular; // Not required
};

// Lights
uniform Material u_Material;
uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLights[POINT_LIGHTS_MAX]; // TODO/NOTE: With buffers we can add as many lights as we want!
uniform vec3 u_ViewPosition;

// Input vars
in vec3 Normal;
in vec4 Color;
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

// TODO: Rewrite, it doesn't seem to do what it should be doing
// TODO: Use vertex color, material color should apply to vertex color no?
vec3 ProcessDirectionalLight(vec3 normal, vec3 viewDir)
{
	if (!u_DirectionalLight.Enabled)
		return vec3(0.0f);

	vec3 lightDir = normalize(-u_DirectionalLight.Direction);

	// Diffuse shading
	float d = max(dot(normal, lightDir), 0.0f);
	
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float s = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
	
	// Calculate final lighting
	vec3 ambient = vec3(0.0f);
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);

	if (!u_Material.TextureAmbientEnabled) ambient = u_DirectionalLight.Ambient * u_Material.Ambient;
	else ambient = u_DirectionalLight.Ambient * texture(u_Material.TextureAmbient, TexCoords);
	if (!u_Material.TextureDiffuseEnabled) diffuse = u_DirectionalLight.Diffuse * d * u_Material.Diffuse;
	else diffuse = u_DirectionalLight.Diffuse * d * texture(u_Material.TextureDiffuse, TexCoords);
	if (!u_Material.TextureSpecularEnabled) specular = u_DirectionalLight.Specular * s * u_Material.Specular;
	else specular = u_DirectionalLight.Specular * s * texture(u_Material.TextureSpecular, TexCoords);

	return (ambient + diffuse + specular) * u_DirectionalLight.Intensity;
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

	// Set color
	FragColor = vec4(color, 1.0f);
}