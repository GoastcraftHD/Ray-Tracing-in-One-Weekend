#pragma once

#include <algorithm>

#include "Utils.h"
#include "Hittable.h"
#include "HittableList.h"

class BVHNode : public Hittable
{
public:
	BVHNode(const HittableList& list) : BVHNode(list.objects, 0, list.objects.size()) {}

	BVHNode(const std::vector<std::shared_ptr<Hittable>>& srcObjects, size_t start, size_t end)
	{
		std::vector<std::shared_ptr<Hittable>> objects = srcObjects;

		int axis = RandomInt(0, 2);
		auto comparator = (axis == 0) ? BoxXCompare
						: (axis == 1) ? BoxYCompare
						: BoxZCompare;

		size_t objectSpan = end - start;

		if (objectSpan == 1)
		{
			m_Left = m_Right = objects[start];
		}
		else if (objectSpan == 2)
		{
			if (comparator(objects[start], objects[start + 1]))
			{
				m_Left = objects[start];
				m_Right = objects[start + 1];
			}
			else
			{
				m_Left = objects[start + 1];
				m_Right = objects[start];
			}
		}
		else
		{
			std::sort(objects.begin() + start, objects.begin() + end, comparator);

			size_t mid = start + objectSpan / 2;
			m_Left = std::make_shared<BVHNode>(objects, start, mid);
			m_Right = std::make_shared<BVHNode>(objects, mid, end);
		}

		m_Bbox = AABB(m_Left->BoundingBox(), m_Right->BoundingBox());
	}

	bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const override
	{
		if (!m_Bbox.Hit(ray, rayT))
			return false;

		bool hitLeft = m_Left->Hit(ray, rayT, hit);
		bool hitRight = m_Right->Hit(ray, Interval(rayT.Min, hitLeft ? hit.T : rayT.Max), hit);

		return hitLeft || hitRight;
	}

	AABB BoundingBox() const override { return m_Bbox; }

private:
	std::shared_ptr<Hittable> m_Left;
	std::shared_ptr<Hittable> m_Right;
	AABB m_Bbox;

	static bool BoxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axisIndex)
	{
		return a->BoundingBox().Axis(axisIndex).Min < b->BoundingBox().Axis(axisIndex).Min;
	}

	static bool BoxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
	{
		return BoxCompare(a, b, 0);
	}

	static bool BoxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
	{
		return BoxCompare(a, b, 1);
	}

	static bool BoxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
	{
		return BoxCompare(a, b, 2);
	}
};
