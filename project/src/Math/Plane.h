#pragma once

#include <glm/glm.hpp>

// From: https://github.com/sharpdx/SharpDX
enum PlaneIntersectionType
{
	kPlaneIntersectionType_Back,
	kPlaneIntersectionType_Front,
	kPlaneIntersectionType_Intersecting
};

class Plane
{
	glm::vec3 m_Normal;
	float m_Distance;

public:
	Plane();
	Plane(float a, float b, float c, float d);
	Plane(glm::vec3 point, glm::vec3 normal);
	Plane(glm::vec3 normal, float d);

	const glm::vec3 &GetNormal() const;
	void SetNormal(const glm::vec3 &normal);
	const float &GetDistance() const;
	void SetDistance(const float &distance);

	void Normalize();
	PlaneIntersectionType Intersects(const glm::vec3 &point) const;
};