#pragma once

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable
{
public:
	Sphere(Vec3 center, double radius, std::shared_ptr<Material> material) : m_Center(center), m_Radius(radius), m_Material(material) {}

	bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const override
	{
		Vec3 oc = ray.Origin() - m_Center;
		double a = ray.Direction().length_squared();
		double halfB = Dot(oc, ray.Direction());
		double c = oc.length_squared() - m_Radius * m_Radius;
		double discriminant = halfB * halfB - a * c;

		if (discriminant < 0)
			return false;

		double sqrtDiscriminant = sqrt(discriminant);
		double root = (-halfB - sqrtDiscriminant) / a;

		if (!rayT.Surrounds(root))
		{
			root = (-halfB + sqrtDiscriminant) / a;

			if (!rayT.Surrounds(root))
			{
				return false;
			}
		}

		hit.T = root;
		hit.Point = ray.At(hit.T);
		hit.Material = m_Material;

		Vec3 outwardNormal = (hit.Point - m_Center) / m_Radius;
		hit.SetFaceNormal(ray, outwardNormal);

		return true;
	}

private:
	Vec3 m_Center;
	double m_Radius;
	std::shared_ptr<Material> m_Material;
};