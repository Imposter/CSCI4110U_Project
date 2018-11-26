#include "BoundingFrustum.h"

void BoundingFrustum::GetPlanesFromMatrix(const glm::mat4x4 &matrix, Plane &near, Plane &far, Plane &left, Plane &right,
	Plane &top, Plane &bottom)
{
	// From: https://github.com/sharpdx/SharpDX
	// http://www.chadvernon.com/blog/resources/directx9/frustum-culling/

	// Left plane
	left.SetNormal({
		matrix[0][3] + matrix[0][0],
		matrix[1][3] + matrix[1][0],
		matrix[2][3] + matrix[2][0]
	});
	left.SetDistance(matrix[3][3] + matrix[3][0]);
	left.Normalize();

	// Right plane
	right.SetNormal({
		matrix[0][3] - matrix[0][0],
		matrix[1][3] - matrix[1][0],
		matrix[2][3] - matrix[2][0]
	});
	right.SetDistance(matrix[3][3] - matrix[3][0]);
	right.Normalize();

	// Top plane
	top.SetNormal({
		matrix[0][3] - matrix[0][1],
		matrix[1][3] - matrix[1][1],
		matrix[2][3] - matrix[2][1]
	});
	top.SetDistance(matrix[3][3] - matrix[3][1]);
	top.Normalize();

	// Bottom plane
	bottom.SetNormal({
		matrix[0][3] + matrix[0][1],
		matrix[1][3] + matrix[1][1],
		matrix[2][3] + matrix[2][1]
	});
	bottom.SetDistance(matrix[3][3] + matrix[3][1]);
	bottom.Normalize();

	// Near plane
	near.SetNormal({
		matrix[0][2],
		matrix[1][2],
		matrix[2][2]
	});
	near.SetDistance(matrix[3][2]);
	near.Normalize();

	// Far plane
	far.SetNormal({
		matrix[0][3] - matrix[0][2],
		matrix[1][3] - matrix[1][2],
		matrix[2][3] - matrix[2][2]
	});
	far.SetDistance(matrix[3][3] - matrix[3][2]);
	far.Normalize();
}

BoundingFrustum::BoundingFrustum(glm::mat4x4 matrix)
{
	GetPlanesFromMatrix(matrix, m_Near, m_Far, m_Left, m_Right, m_Top, m_Bottom);
}

ContainmentType BoundingFrustum::Contains(const glm::vec3 &vec) const
{
	// From: https://github.com/sharpdx/SharpDX
	// http://www.chadvernon.com/blog/resources/directx9/frustum-culling/
	auto res = kPlaneIntersectionType_Front;
	auto planeRes = kPlaneIntersectionType_Front;
	for (auto i = 0; i < 6; i++)
	{
		if (i == 0) planeRes = m_Near.Intersects(vec);
		else if (i == 1) planeRes = m_Far.Intersects(vec);
		else if (i == 2) planeRes = m_Left.Intersects(vec);
		else if (i == 3) planeRes = m_Right.Intersects(vec);
		else if (i == 4) planeRes = m_Top.Intersects(vec);
		else if (i == 5) planeRes = m_Bottom.Intersects(vec);

		if (planeRes == kPlaneIntersectionType_Back)
			return kContainmentType_Disjoint;
		if (planeRes == kPlaneIntersectionType_Intersecting)
		{
			res = kPlaneIntersectionType_Intersecting;
			break;
		}
	}

	if (res == kPlaneIntersectionType_Intersecting)
		return kContainmentType_Intersects;

	return kContainmentType_Contains;
}
