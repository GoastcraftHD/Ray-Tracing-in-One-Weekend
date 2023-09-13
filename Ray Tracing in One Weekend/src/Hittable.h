#pragma once

#include "Utils.h"
#include "Ray.h"

class Material;

struct HitRecord
{
	Vec3 Point;
	Vec3 Normal;
	std::shared_ptr<Material> Material;
	double T;
	bool FrontFace;

	void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
	{
		FrontFace = Dot(ray.Direction(), outwardNormal) < 0;
		Normal = FrontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const = 0;
};