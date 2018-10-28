#include "Transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

void Transform::applyOperation(const Transform *transform, glm::mat4 &matrix, Operation operation)
{
	if (transform->m_Parent)
		applyOperation(transform->m_Parent, matrix, operation);

	switch (operation)
	{
	case kOperation_Position:
		matrix = glm::translate(matrix, transform->m_Position);
		break;
	case kOperation_Rotation:
		matrix *= glm::toMat4(transform->m_Rotation);
		break;
	}
}

void Transform::rotationMatrixToEuler(glm::mat4 &m, glm::vec3 &v)
{
	const auto sy = sqrt(m[0][0] * m[0][0] + m[1][0] * m[1][0]);
	if (sy < 1e-6)
	{
		v.x = atan2(-m[1][2], m[1][1]);
		v.y = atan2(-m[2][0], sy);
		v.z = 0;
	}
	else
	{
		v.x = atan2(m[2][1], m[2][2]);
		v.y = atan2(-m[2][0], sy);
		v.z = atan2(m[1][0], m[0][0]);
	}
}

void Transform::update()
{
	// Reset transform
	m_Matrix = glm::mat4(1.0f);

	// Get total position
	glm::mat4 positionTransform(1.0f);
	applyOperation(this, positionTransform, kOperation_Position);

	// Get total rotation
	glm::mat4 rotationTransform(1.0f);
	applyOperation(this, rotationTransform, kOperation_Rotation);

	// Get position from position matrix
	const glm::vec3 position(positionTransform[3]);

	// Get rotation from rotation matrix
	glm::vec3 rotation(0.0f);
	rotationMatrixToEuler(rotationTransform, rotation);

	// Apply rotation
	m_Matrix = glm::rotate(m_Matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_Matrix = glm::rotate(m_Matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_Matrix = glm::rotate(m_Matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	// Apply translation
	m_Matrix = glm::translate(m_Matrix, position);

	// Apply scale
	m_Matrix = glm::scale(m_Matrix, m_Scale);
}

Transform::Transform(Transform *parent)
	: m_Position(0.0f), m_Rotation(0.0f, 0.0f, 0.0f, 0.0f), m_Scale(1.0f), m_Matrix(1.0f), m_Parent(parent)
{
}

Transform *Transform::GetParent() const
{
	return m_Parent;
}

void Transform::SetParent(Transform *parent)
{
	m_Parent = parent;
	update();
}

const glm::vec3 &Transform::GetPosition() const
{
	return m_Position;
}

void Transform::SetPosition(const glm::vec3 &position)
{
	m_Position = position;
	update();
}

const glm::vec3 &Transform::GetScale() const
{
	return m_Scale;
}

void Transform::SetScale(const glm::vec3 &scale)
{
	m_Scale = scale;
	update();
}

const glm::quat &Transform::GetRotation() const
{
	return m_Rotation;
}

void Transform::SetRotation(const glm::quat &rotation)
{
	m_Rotation = rotation;
	update();
}

// TODO/NOTE: We might need to work on directions, move camera to top to look down, and design coordinates
glm::vec3 Transform::Up() const
{
	return m_Matrix[1];
}

glm::vec3 Transform::Forward() const
{
	return m_Matrix[2];
}

glm::vec3 Transform::Right() const
{
	return m_Matrix[0];
}

const glm::mat4 &Transform::GetMatrix() const
{
	return m_Matrix;
}
