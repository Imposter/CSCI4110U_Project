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

// TODO: Add transform? so we can have a class where we can add transition effects (- lookAt)
// like m_TransitionQueue; pop transition and lerp rotation/position to it over a duration to show difference in lighting

// TODO: Texture class

class Camera : public Object
{
	float m_FOV; // Same as Zoom
	float m_NearPlane;
	float m_FarPlane;

	glm::vec4 m_ClearColor;
	float m_ClearDepth;
	CameraClearMode m_ClearMode;

	BoundingFrustum m_BoundingFrustum;

public:
	Camera();
	~Camera();

	// TODO: Complete based on: https://learnopengl.com/Getting-started/Camera
	// TODO: Complete based on: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h

	void Update(float deltaTime) override;
	void Render(float deltaTime) override;
	void Shutdown() override;

	
/*public:
	void Render(); // -- ?? should take root renderable object

	const glm::mat4x4 &GetPerspectiveMatrix() const;
	const glm::mat4x4 &GetViewMatrix() const;

	const BoundingFrustum &GetBoundingFrustum() const;*/
};