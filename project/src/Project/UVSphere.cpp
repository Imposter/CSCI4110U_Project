#define _USE_MATH_DEFINES
#include <cmath>
#include "UVSphere.h"

UVSphere::UVSphere(unsigned int vertRes, unsigned int horizRes, float radius)
	: m_NumVerticalSegments(vertRes), m_NumHorizontalSegments(horizRes), m_Radius(radius)
{
	const auto deltaTheta = M_PI * 2.0f / m_NumVerticalSegments;
	const auto deltaPhi = M_PI * 2.0f / m_NumHorizontalSegments;

	const glm::vec3 centre(0.0f);

	for (unsigned int latitudeIndex = 0; latitudeIndex < m_NumHorizontalSegments; latitudeIndex++)
	{
		const auto phi = -1.0f * M_PI + latitudeIndex * deltaPhi;

		// Generate one circle at this latitude
		for (unsigned int longitudeIndex = 0; longitudeIndex < m_NumVerticalSegments; longitudeIndex++)
		{
			const auto theta = longitudeIndex * deltaTheta;

			// Generate one vertex
			const auto x = m_Radius * cos(theta) * sin(phi);
			const auto y = m_Radius * cos(phi);
			const auto z = m_Radius * sin(theta) * sin(phi);

			const glm::vec3 pos(x, y, z);
			m_Positions.push_back(pos);

			// Calculate texture coordinates, using the spherical coordinates
			const auto u = theta / (2.0f * M_PI);
			const auto v = (phi + M_PI) / (2.0f * M_PI);
			const glm::vec2 uv(u, v);
			m_TextureCoords.push_back(uv);

			// calculate the surface normals
			const auto normal = glm::normalize(pos - centre);
			m_Normals.push_back(normal);

			if (latitudeIndex > 0 && longitudeIndex > 0)
			{
				const auto a = latitudeIndex * m_NumHorizontalSegments + longitudeIndex;
				const auto b = latitudeIndex * m_NumHorizontalSegments + (longitudeIndex - 1);
				const auto c = (latitudeIndex - 1) * m_NumHorizontalSegments + longitudeIndex;
				const auto d = (latitudeIndex - 1) * m_NumHorizontalSegments + (longitudeIndex - 1);

				m_Indices.push_back(a);
				m_Indices.push_back(b);
				m_Indices.push_back(c);

				m_Indices.push_back(d);
				m_Indices.push_back(c);
				m_Indices.push_back(b);
			}
		}
	}

	// Handle the top special case
	{
		const auto c = m_Positions.size();
		const glm::vec3 middleCentre(0.0, m_Radius, 0.0);
		m_Positions.push_back(middleCentre);

		// Calculate texture coordinates, using the spherical coordinates
		const glm::vec2 uv(0.5, 1.0);
		m_TextureCoords.push_back(uv);

		// Calculate the surface normals
		const glm::vec3 normal = glm::normalize(middleCentre - centre);
		m_Normals.push_back(normal);

		const auto row = m_NumVerticalSegments - 1;
		for (unsigned int col = 1; col < m_NumHorizontalSegments; col++)
		{
			// Form a triangle between col, col-1, and the top centre
			const auto a = row * m_NumHorizontalSegments + col;
			const auto b = row * m_NumHorizontalSegments + (col - 1);

			m_Indices.push_back(a);
			m_Indices.push_back(b);
			m_Indices.push_back(c);
		}
	}

	// Handle the bottom special case
	{
		const auto c = m_Positions.size();
		const glm::vec3 middleCentre(0.0, -m_Radius, 0.0);
		m_Positions.push_back(middleCentre);

		// Calculate texture coordinates, using the spherical coordinates
		const glm::vec2 uv(0.5, 0.0);
		m_TextureCoords.push_back(uv);

		// Calculate the surface normals
		const glm::vec3 normal = glm::normalize(middleCentre - centre);
		m_Normals.push_back(normal);

		const auto row = m_NumVerticalSegments - 1;
		for (unsigned int col = 1; col < m_NumHorizontalSegments; col++)
		{
			// Form a triangle between col, col-1, and the bottom centre
			const auto a = col;
			const auto b = col - 1;

			m_Indices.push_back(c);
			m_Indices.push_back(b);
			m_Indices.push_back(a);
		}
	}

	// Handle the last column special case
	{
		const auto lastCol = m_NumHorizontalSegments - 1;
		for (unsigned int row = 1; row < m_NumVerticalSegments; row++)
		{
			const auto a = (row - 1) * m_NumHorizontalSegments;
			const auto b = (row - 1) * m_NumHorizontalSegments + lastCol;
			const auto c = row * m_NumHorizontalSegments;
			const auto d = row * m_NumHorizontalSegments + lastCol;

			m_Indices.push_back(b);
			m_Indices.push_back(d);
			m_Indices.push_back(a);

			m_Indices.push_back(d);
			m_Indices.push_back(b);
			m_Indices.push_back(c);
		}
	}
}

const std::vector<glm::vec3> &UVSphere::GetPositions() const
{
	return m_Positions;
}

const std::vector<glm::vec2> &UVSphere::GetTextureCoords()  const
{
	return m_TextureCoords;
}

const std::vector<glm::vec3> &UVSphere::GetNormals() const
{
	return m_Normals;
}

const std::vector<unsigned> & UVSphere::GetIndices() const
{
	return m_Indices;
}

float UVSphere::GetRadius() const
{
	return m_Radius;
}

unsigned int UVSphere::GetNumVerticalSegments() const
{
	return m_NumVerticalSegments;
}

unsigned int UVSphere::GetNumHorizontalSegments() const
{
	return m_NumHorizontalSegments;
}