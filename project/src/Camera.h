#pragma once

#include "Transform.h"
#include "Math/BoundingFrustum.h"
#include "Node.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

enum CameraClearMode
{
	kCameraClearMode_None,
	kCameraClearMode_Color = GL_COLOR_BUFFER_BIT,
	kCameraClearMode_Depth = GL_DEPTH_BUFFER_BIT,
	kCameraClearMode_Both = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
};

class Camera
{
	GraphicsManager *m_GraphicsManager;

	float m_FOV; // Same as Zoom
	float m_NearPlane;
	float m_FarPlane;
	float m_AspectRatio;

	glm::vec4 m_ClearColor;
	float m_ClearDepth;
	CameraClearMode m_ClearMode;

	Transform m_Transform;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	// Shaders
	std::vector<Shader *> m_Shaders;

public:
	Camera(float fov, float near, float far, float aspectRatio, GraphicsManager *graphicsManager);
	~Camera();

	const glm::vec4 &GetClearColor() const;
	void SetClearColor(const glm::vec4 &color);

	float GetClearDepth() const;
	void SetClearDepth(float depth);

	CameraClearMode GetClearMode() const;
	void SetClearMode(CameraClearMode mode);

	Transform *GetTransform();

	void Move(const glm::vec3 &v, float val);
	void Rotate(const glm::vec3 &v, float radians);
	void LookAt(const glm::vec3 &target, const glm::vec3 &position, const glm::vec3 &up);

	float GetAspectRatio() const;
	void SetAspectRatio(float aspectRatio);

	const glm::mat4 &GetProjectionMatrix() const;
	const glm::mat4 &GetViewMatrix() const;

	void SetModelTransform(Transform *transform);

	BoundingFrustum GetBoundingFrustum() const;

	void Update(float deltaTime);
	void Render(Node *node, float deltaTime);
};