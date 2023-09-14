#pragma once

#include "Utils.h"
#include "Ray.h"
#include "AABB.h"

class Material;

struct HitRecord
{
	glm::vec3 Point;
	glm::vec3 Normal;
	std::shared_ptr<Material> Material;
	float T;
	bool FrontFace;

	void SetFaceNormal(const Ray& ray, const glm::vec3& outwardNormal)
	{
		FrontFace = glm::dot(ray.Direction(), outwardNormal) < 0.0f;
		Normal = FrontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const = 0;
	virtual AABB BoundingBox() const = 0;
};
