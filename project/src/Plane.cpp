#include "Plane.h"
#include <glm/geometric.hpp>
#include <cmath>

Plane::Plane()
	: m_Normal(0), m_Distance(0)
{
}

Plane::Plane(float a, float b, float c, float d)
	: m_Normal(a, b, c), m_Distance(d)
{
}

Plane::Plane(glm::vec3 point, glm::vec3 normal)
	: m_Normal(normal), m_Distance(-dot(normal, point))
{

}

Plane::Plane(glm::vec3 normal, float d)
	: m_Normal(normal), m_Distance(d)
{
}

const glm::vec3 & Plane::GetNormal() const
{
	return m_Normal;
}

void Plane::SetNormal(const glm::vec3 &normal)
{
	m_Normal = normal;
}

const float & Plane::GetDistance() const
{
	return m_Distance;
}

void Plane::SetDistance(const float &distance)
{
	m_Distance = distance;
}

void Plane::Normalize()
{
	const auto magnitude = 1.0f / sqrt(m_Normal.x * m_Normal.x + m_Normal.y * m_Normal.y + m_Normal.z * m_Normal.z);

	m_Normal.x *= magnitude;
	m_Normal.y *= magnitude;
	m_Normal.z *= magnitude;
	m_Distance *= magnitude;
}

PlaneIntersectionType Plane::Intersects(const glm::vec3 &point) const
{
	const auto distance = dot(m_Normal, point) + m_Distance;

	if (distance > 0.0f)
		return kPlaneIntersectionType_Front;

	if (distance < 0.0f)
		return kPlaneIntersectionType_Back;

	return kPlaneIntersectionType_Intersecting;
}
