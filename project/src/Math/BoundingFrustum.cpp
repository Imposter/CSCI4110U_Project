#include "BoundingFrustum.h"

void BoundingFrustum::GetPlanesFromMatrix(const glm::mat4x4 &matrix, Plane &near, Plane &far, Plane &left, Plane &right,
	Plane &top, Plane &bottom)
{
	// From: https://github.com/sharpdx/SharpDX
	// http://www.chadvernon.com/blog/resources/directx9/frustum-culling/

	// Left plane
	left.SetNormal({
		matrix[1][4] + matrix[1][1],
		matrix[2][4] + matrix[2][1],
		matrix[3][4] + matrix[3][1]
	});
	left.SetDistance(matrix[4][4] + matrix[4][1]);
	left.Normalize();

	// Right plane
	right.SetNormal({
		matrix[1][4] - matrix[1][1],
		matrix[2][4] - matrix[2][1],
		matrix[3][4] - matrix[3][1]
	});
	right.SetDistance(matrix[4][4] - matrix[4][1]);
	right.Normalize();

	// Top plane
	top.SetNormal({
		matrix[1][4] - matrix[1][2],
		matrix[2][4] - matrix[2][2],
		matrix[3][4] - matrix[3][2]
	});
	top.SetDistance(matrix[4][4] - matrix[4][2]);
	top.Normalize();

	// Bottom plane
	bottom.SetNormal({
		matrix[1][4] + matrix[1][2],
		matrix[2][4] + matrix[2][2],
		matrix[3][4] + matrix[3][2]
	});
	bottom.SetDistance(matrix[4][4] + matrix[4][2]);
	bottom.Normalize();

	// Near plane
	near.SetNormal({
		matrix[1][3],
		matrix[2][3],
		matrix[3][3]
	});
	near.SetDistance(matrix[4][3]);
	near.Normalize();

	// Far plane
	far.SetNormal({
		matrix[1][4] - matrix[1][3],
		matrix[2][4] - matrix[2][3],
		matrix[3][4] - matrix[3][3]
	});
	far.SetDistance(matrix[4][4] - matrix[4][3]);
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
