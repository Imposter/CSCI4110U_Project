#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
	enum Operation
	{
		kOperation_Position,
		kOperation_Rotation
	};

	glm::vec3 m_Position;
	glm::quat m_Rotation;
	glm::vec3 m_Scale;

	glm::mat4 m_Matrix;

	Transform *m_Parent;

	static void applyOperation(const Transform *transform, glm::mat4 &matrix, Operation operation);

	void update();
	
public:
	Transform(Transform *parent = nullptr);

	Transform *GetParent() const;
	void SetParent(Transform *parent);

	const glm::vec3 &GetPosition() const;
	void SetPosition(const glm::vec3 &position);
	void OffsetPosition(const glm::vec3 &offset);

	const glm::vec3 &GetScale() const;
	void SetScale(const glm::vec3 &scale);

	const glm::quat &GetRotation() const;
	void SetRotation(const glm::quat &rotation);
	void SetRotation(const glm::vec3 &v, float radians);
	void OffsetRotation(const glm::vec3 &v, float radians);

	void LookAt(const glm::vec3 &pos);

	glm::vec3 Up() const;
	glm::vec3 Forward() const;
	glm::vec3 Right() const;

	const glm::mat4 &GetMatrix() const;
};