#pragma once

#include <memory>
#include <vector>

#include "Hittable.h"
#include "AABB.h"

class HittableList : public Hittable
{
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object) { Add(object); }

	void Clear() { objects.clear(); }
	void Add(std::shared_ptr<Hittable> object)
	{
		objects.push_back(object);
		m_Bbox = AABB(m_Bbox, object->BoundingBox());
	}

	bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const override
	{
		HitRecord tempHit;
		bool hitAnything = false;
		float closestSoFar = rayT.Max;

		for (const std::shared_ptr<Hittable>& object : objects)
		{
			if (object->Hit(ray, Interval(rayT.Min, closestSoFar), tempHit))
			{
				hitAnything = true;
				closestSoFar = tempHit.T;
				hit = tempHit;
			}
		}

		return hitAnything;
	}

	AABB BoundingBox() const override { return m_Bbox; }

private:
	AABB m_Bbox;
};
