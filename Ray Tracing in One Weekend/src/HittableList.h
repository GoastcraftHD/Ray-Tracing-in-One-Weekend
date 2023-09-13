#pragma once

#include <memory>
#include <vector>

#include "Hittable.h"

class HittableList : public Hittable
{
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object) { Add(object); }

	void Clear() { objects.clear(); }
	void Add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

	bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const override
	{
		HitRecord tempHit;
		bool hitAnything = false;
		double closestSoFar = rayT.Max;

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
};