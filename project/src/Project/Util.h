#pragma once

#include <cstdlib>
#include <glm/glm.hpp>

inline float RandomFloat(float minVal, float maxVal) 
{
	return minVal + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxVal - minVal);
}

inline int RandomInt(int minVal, int maxVal)
{
	return minVal + rand() % (maxVal - minVal);
}