#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(float fov, float near, float far, float aspectRatio, GraphicsManager *graphicsManager)
	: m_GraphicsManager(graphicsManager), m_FOV(fov), m_NearPlane(near), m_FarPlane(far), m_AspectRatio(aspectRatio), m_ClearColor(0.0f), 
	m_ClearDepth(1.0f), m_ClearMode(kCameraClearMode_None), m_ProjectionMatrix(0.0f), m_ViewMatrix(0.0f)
{
	// Store shaders
	m_Shaders.push_back(m_GraphicsManager->GetShader("Flat"));
	m_Shaders.push_back(m_GraphicsManager->GetShader("Lambert"));
}

Camera::~Camera() = default;

const glm::vec4 &Camera::GetClearColor() const
{
	return m_ClearColor;
}

void Camera::SetClearColor(const glm::vec4 &color)
{
	m_ClearColor = color;
}

float Camera::GetClearDepth() const
{
	return m_ClearDepth;
}

void Camera::SetClearDepth(float depth)
{
	m_ClearDepth = depth;
}

CameraClearMode Camera::GetClearMode() const
{
	return m_ClearMode;
}

void Camera::SetClearMode(CameraClearMode mode)
{
	m_ClearMode = mode;
}

Transform *Camera::GetTransform()
{
	return &m_Transform;
}

float Camera::GetAspectRatio() const
{
	return m_AspectRatio;
}

void Camera::SetAspectRatio(float aspectRatio)
{
	m_AspectRatio = aspectRatio;
}

const glm::mat4x4 &Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

const glm::mat4x4 &Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

void Camera::SetModelTransform(Transform *transform)
{
	// Update shaders
	for (const auto &s : m_Shaders)
	{
		s->Use();
		const auto modelVar = s->GetVariable("u_Model");

		// Set transform matrix
		modelVar->SetMat4(transform->GetMatrix());
	}
}

BoundingFrustum Camera::GetBoundingFrustum() const
{
	return{ m_Transform.GetMatrix() };
}

void Camera::Update(float deltaTime)
{
	// Set view matrix from transform
	m_ViewMatrix = m_Transform.GetMatrix();

	// Set projection matrix
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
}

void Camera::Render(Node *node, float deltaTime, bool clear)
{
	if (clear)
	{
		// Clear buffer
		if (m_ClearMode != kCameraClearMode_None)
		{
			if (m_ClearMode & kCameraClearMode_Depth)
			{
				// Set clear depth
				glClearDepthf(m_ClearDepth);
			}
			if (m_ClearMode & kCameraClearMode_Color)
			{
				// Set clear color
				glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
			}

			// Clear
			glClear(m_ClearMode);
		}
	}

	// Create render context
	RenderContext rc{};
	rc.GraphicsManager = m_GraphicsManager;
	rc.Camera = this;

	// Update shaders
	for (const auto &s : m_Shaders)
	{
		s->Use();
		const auto viewVar = s->GetVariable("u_View");
		const auto projVar = s->GetVariable("u_Projection");
			
		viewVar->SetMat4(m_ViewMatrix);
		projVar->SetMat4(m_ProjectionMatrix);
	}
	
	// Render nodes
	if (node->IsActive())
		node->Render(&rc);
}