#pragma once

#include "Utils.h"

class AABB
{
public:
	Interval X, Y, Z;

	AABB() {}
	AABB(const Interval& ix, const Interval& iy, const Interval& iz) : X(ix), Y(iy), Z(iz) {}

	AABB(const glm::vec3& a, const glm::vec3& b)
	{
		X = Interval(fmin(a.x, b.x), fmax(a.x, b.x));
		Y = Interval(fmin(a.y, b.y), fmax(a.y, b.y));
		Z = Interval(fmin(a.z, b.z), fmax(a.z, b.z));
	}

	AABB(const AABB& box0, const AABB& box1)
	{
		X = Interval(box0.X, box1.X);
		Y = Interval(box0.Y, box1.Y);
		Z = Interval(box0.Z, box1.Z);
	}

	AABB Pad()
	{
		float delta = 0.0001;
		Interval newX = X.Size() >= delta ? X : X.Expand(delta);
		Interval newY = Y.Size() >= delta ? Y : Y.Expand(delta);
		Interval newZ = Z.Size() >= delta ? Z : Z.Expand(delta);

		return AABB(newX, newY, newZ);
	}

	const Interval& Axis(int n) const
	{
		if (n == 1) return Y;
		if (n == 2) return Z;
		return X;
	}

	bool Hit(const Ray& ray, Interval rayT) const
	{
		for (int a = 0; a < 3; a++)
		{
			float invD = 1 / ray.Direction()[a];
			float orig = ray.Origin()[a];

			float t0 = (Axis(a).Min - orig) * invD;
			float t1 = (Axis(a).Max - orig) * invD;

			if (invD < 0)
				std::swap(t0, t1);

			if (t0 > rayT.Min) rayT.Min = t0;
			if (t1 < rayT.Max) rayT.Max = t1;

			if (rayT.Max <= rayT.Min)
				return false;
		}

		return true;
	}
};

AABB operator+(const AABB& bbox, const glm::vec3& offset)
{
	return AABB(bbox.X + offset.x, bbox.Y + offset.y, bbox.Z + offset.z);
}

AABB operator+(const glm::vec3& offset, const AABB& bbox)
{
	return bbox + offset;
}
