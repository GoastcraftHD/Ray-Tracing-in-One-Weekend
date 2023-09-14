#pragma once

#include "Utils.h"

class Ray
{
public:
	Ray() {}

	Ray(const glm::vec3& origin, const glm::vec3& direction) : m_Origin(origin), m_Direction(direction), m_Time(0.0f) {}
	Ray(const glm::vec3& origin, const glm::vec3& direction, float time = 0.0f) : m_Origin(origin), m_Direction(direction), m_Time(time) {}

	glm::vec3 Origin() const { return m_Origin; }
	glm::vec3 Direction() const { return m_Direction; }
	float Time() const { return m_Time; }

	glm::vec3 At(float t) const
	{
		return m_Origin + t * m_Direction;
	}

private:
	glm::vec3 m_Origin;
	glm::vec3 m_Direction;
	float m_Time;
};
