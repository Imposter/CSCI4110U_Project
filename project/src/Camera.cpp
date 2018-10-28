#include "Camera.h"

// TODO: Finish Camera, start on Texture and Light/DirectionalLight/SpotLight/PointLight classes

Camera::Camera()
	: m_FOV(60.0f), m_NearPlane(0.0f), m_FarPlane(1.0f), m_ClearColor(0.0f), m_ClearDepth(0.0f), m_ClearMode(kCameraClearMode_Both)
{
}

Camera::~Camera() = default;

const glm::mat4x4 &Camera::GetPerspectiveMatrix() const
{
	return m_PerspectiveMatrix;
}

void Camera::SetPerspectiveMatrix(const glm::mat4 &m)
{
	m_PerspectiveMatrix = m;
}

const glm::mat4x4 &Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

void Camera::SetViewMatrix(const glm::mat4 &m)
{
	m_ViewMatrix = m;
}

BoundingFrustum Camera::GetBoundingFrustum() const
{
	return{ m_Transform.GetMatrix() };
}

void Camera::Update(float deltaTime) {}
void Camera::Render(float deltaTime) {}
void Camera::Shutdown() {}