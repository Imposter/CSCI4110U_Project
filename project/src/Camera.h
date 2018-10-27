#pragma once

#include "BoundingFrustum.h"
#include <glm/matrix.hpp>

enum CameraClearMode
{
	kCameraClearMode_None,
	kCameraClearMode_Color,
	kCameraClearMode_Depth,
	kCameraClearMode_Both
};

class Camera
{
	float m_FOV;
	glm::mat4x4 m_Matrix;
	float m_NearPlane;
	float m_FarPlane;
	glm::vec4 m_ClearColor;
	float m_ClearDepth;
	CameraClearMode m_ClearMode;
	BoundingFrustum m_BoundingFrustum;

	// TODO: Take GraphicsManager ??
	
public:
	void Render(); // -- ?? should take root renderable object

	const glm::mat4x4 &GetPerspectiveMatrix() const;
	const glm::mat4x4 &GetViewMatrix() const;

	const BoundingFrustum &GetBoundingFrustum() const;
};