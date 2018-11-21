#pragma once

#include "Plane.h"
#include <glm/glm.hpp>

enum ContainmentType
{
	kContainmentType_Disjoint,
	kContainmentType_Contains,
	kContainmentType_Intersects
};

// From: https://github.com/sharpdx/SharpDX
class BoundingFrustum
{
	Plane m_Near;
	Plane m_Far;
	Plane m_Left;
	Plane m_Right;
	Plane m_Top;
	Plane m_Bottom;

	static void GetPlanesFromMatrix(const glm::mat4x4 &matrix, Plane &near, Plane &far, Plane &left, Plane &right, Plane &top, Plane &bottom);

public:
	BoundingFrustum(glm::mat4x4 matrix);

	ContainmentType Contains(const glm::vec3 &vec) const;
};