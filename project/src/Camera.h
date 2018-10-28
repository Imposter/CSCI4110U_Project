#pragma once

#include "Object.h"
#include "Math/BoundingFrustum.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

enum CameraClearMode
{
	kCameraClearMode_None,
	kCameraClearMode_Color,
	kCameraClearMode_Depth,
	kCameraClearMode_Both
};

// TODO: Texture class

class Camera : public Object
{
	float m_FOV; // Same as Zoom
	float m_NearPlane;
	float m_FarPlane;

	glm::vec4 m_ClearColor;
	float m_ClearDepth;
	CameraClearMode m_ClearMode;

	glm::mat4 m_PerspectiveMatrix;
	glm::mat4 m_ViewMatrix;

public:
	Camera();
	~Camera();

	// TODO: Complete based on: https://learnopengl.com/Getting-started/Camera
	// TODO: Complete based on: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h

	const glm::mat4 &GetPerspectiveMatrix() const;
	void SetPerspectiveMatrix(const glm::mat4 &m);

	const glm::mat4 &GetViewMatrix() const;
	void SetViewMatrix(const glm::mat4 &m);

	BoundingFrustum GetBoundingFrustum() const;

	void Update(float deltaTime) override; // TODO/NOTE: How will we implement clearing, etc?
	void Render(float deltaTime) override;
	void Shutdown() override;
};