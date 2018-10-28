#include "Camera.h"

// TODO: Finish Camera today, start on Texture and Light/DirectionalLight/SpotLight/PointLight classes

Camera::Camera()
	: m_FOV(60.0f), m_NearPlane(0.0f), m_FarPlane(1.0f), m_ClearColor(0.0f), m_ClearDepth(0.0f), m_ClearMode(kCameraClearMode_Both), m_BoundingFrustum(glm::mat4(0.0f))
{	
}

Camera::~Camera() {}
void Camera::Update(float deltaTime) {}
void Camera::Render(float deltaTime) {}
void Camera::Shutdown() {}