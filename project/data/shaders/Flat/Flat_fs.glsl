#version 330 core

// Type definitions
struct Material
{
	vec3 Ambient; // Unused
	vec3 Diffuse;
	vec3 Specular; // Unused

	float Shininess; // Unused
};

// Obj
uniform Material u_Material;
//uniform vec3 u_ViewPosition; // TODO: Do we need this?

// Output vars
out vec4 FragColor;

void main()
{
	//FragColor = vec4(u_Material.Diffuse, 0.0f) * 0.0001f;
	// TEST DEBUG
	//FragColor = FragColor + vec4(1.0f, 0.0f, 0.0f, 1.0f);
	FragColor = vec4(u_Material.Diffuse, 1.0f);
}