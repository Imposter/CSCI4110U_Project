#pragma once

#include <vector>
#include <glm/glm.hpp>

class UVSphere 
{
	unsigned int m_NumVerticalSegments;
	unsigned int m_NumHorizontalSegments;
	float m_Radius;

	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec2> m_TextureCoords;
	std::vector<glm::vec3> m_Normals;
	std::vector<unsigned int> m_Indices;

public:
	UVSphere(unsigned int vertRes, unsigned int horizRes, float radius);

	const std::vector<glm::vec3> &GetPositions() const;
	const std::vector<glm::vec2> &GetTextureCoords() const;
	const std::vector<glm::vec3> &GetNormals() const;

	const std::vector<unsigned int> &GetIndices() const;

	float GetRadius() const;
	unsigned int GetNumVerticalSegments() const;
	unsigned int GetNumHorizontalSegments() const;
};