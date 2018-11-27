#include "Transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
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

void Transform::update()
{
	// Use basic transformations
	m_Matrix = glm::mat4(1.0f);
	m_Matrix = m_Matrix * glm::toMat4(m_Rotation);
	m_Matrix = m_Matrix * glm::translate(glm::mat4(1.0f), m_Position);
	m_Matrix = glm::scale(m_Matrix, m_Scale);

	// Avoid heirarchical transformations
	/*
	// Get total rotation
	glm::mat4 rotationTransform(1.0f);
	applyOperation(this, rotationTransform, kOperation_Rotation);

	// Get total position
	glm::mat4 positionTransform(1.0f);
	applyOperation(this, positionTransform, kOperation_Position);

	// Build transform matrix
	m_Matrix = rotationTransform;
	m_Matrix[3] = positionTransform[3];
	m_Matrix = glm::scale(m_Matrix, m_Scale);
	*/
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

void Transform::OffsetPosition(const glm::vec3 &offset)
{
	m_Position += offset;
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

void Transform::SetRotation(const glm::vec3 &v, float radians)
{
	glm::mat4 rotationMatrix(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, radians, v);
	m_Rotation = glm::toQuat(rotationMatrix);
	update();
}

void Transform::OffsetRotation(const glm::vec3 &v, float radians)
{
	auto rotationMatrix = glm::toMat4(m_Rotation);
	rotationMatrix = glm::rotate(rotationMatrix, radians, v);
	m_Rotation = glm::toQuat(rotationMatrix);
	update();
}

void Transform::LookAt(const glm::vec3 &pos)
{
	// Create view matrix
	const auto viewMat = glm::lookAt(m_Position, pos, glm::vec3(0.0f, 1.0f, 0.0f));

	// Update transform
	m_Rotation = glm::toQuat(viewMat);
	update();
}

glm::vec3 Transform::Up() const
{
	return -m_Matrix[1];
}

glm::vec3 Transform::Forward() const
{
	return -m_Matrix[2];
}

glm::vec3 Transform::Right() const
{
	return -m_Matrix[0];
}

const glm::mat4 &Transform::GetMatrix() const
{
	return m_Matrix;
}

void Transform::SetMatrix(const glm::mat4 &mat)
{
	m_Matrix = mat;
}
