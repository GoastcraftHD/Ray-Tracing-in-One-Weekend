#pragma once

#include "Vec3.h"

class Ray
{
public:
	Ray() {}

	Ray(const Vec3& origin, const Vec3& direction) : m_Origin(origin), m_Direction(direction) {}

	Vec3 Origin() const { return m_Origin; }
	Vec3 Direction() const { return m_Direction; }

	Vec3 At(double t) const
	{
		return m_Origin + t * m_Direction;
	}

private:
	Vec3 m_Origin;
	Vec3 m_Direction;
};