#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

const float Infinity = std::numeric_limits<float>::infinity();
const float Pi = 3.1415926535897932385f;

inline double RandomFloat() { return rand() / (RAND_MAX + 1.0f); }
inline double RandomFloat(float min, float max) { return min + (max - min) * RandomFloat(); }

// Random Color with the alpha channel set to always be 1
inline glm::vec4 RandomColor() { return glm::vec4(RandomFloat(), RandomFloat(), RandomFloat(), 1.0f); }
inline glm::vec4 RandomColor(float min, float max) { return glm::vec4(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max), 1.0f); }

inline int RandomInt(int min, int max) { return static_cast<int>(RandomFloat(min, max + 1)); }

inline glm::vec3 RandomVector()
{
	return glm::vec3(RandomFloat(), RandomFloat(), RandomFloat());
}

inline glm::vec3 RandomVector(float min, float max)
{
	return glm::vec3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
}

inline bool NearZero(glm::vec3 e)
{
	double s = 1e-8;
	return fabs(e.x) < s && fabs(e.y) < s && fabs(e.z) < s;
}

inline glm::vec3 RandomInUnitDisk()
{
	while (true)
	{
		glm::vec3 p = glm::vec3(RandomFloat(-1, 1), RandomFloat(-1, 1), 0);
		if (glm::length2(p) < 1)
			return p;
	}
}

inline glm::vec3 RandomInUnitSphere()
{
	while (true)
	{
		glm::vec3 p = RandomVector(-1, 1);

		if (glm::length2(p) < 1)
			return p;
	}
}

inline glm::vec3 RandomUnitVector()
{
	return glm::normalize(RandomInUnitSphere());
}

inline glm::vec3 RandomOnHemisphere(const glm::vec3& normal)
{
	glm::vec3 onUnitSphere = RandomUnitVector();

	if (glm::dot(onUnitSphere, normal) > 0.0)
		return onUnitSphere;

	return -onUnitSphere;
}

#include "Interval.h"
#include "Ray.h"
