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

inline float Lerp(float startValue, float endValue, float t)
{
	return startValue * (1.0f - t) + endValue * t;
}

inline glm::vec3 Lerp(glm::vec3 start, glm::vec3 end, float t)
{
	return{
		Lerp(start.x, end.x, t),
		Lerp(start.y, end.y, t),
		Lerp(start.z, end.z, t)
	};
}