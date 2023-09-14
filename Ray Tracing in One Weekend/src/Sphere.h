#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) : m_Center(center), m_Radius(radius), m_Material(material), m_IsMoving(false) {}

	Sphere(const glm::vec3& fromCenter, const glm::vec3& toCenter2, float radius, std::shared_ptr<Material> material) : m_Center(fromCenter), m_Radius(radius), m_Material(material), m_IsMoving(true)
	{
		m_MoveVector = toCenter2 - fromCenter;
	}

	bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const override
	{
		glm::vec3 center = m_IsMoving ? Center(ray.Time()) : m_Center;
		glm::vec3 oc = ray.Origin() - center;
		float a = glm::length2(ray.Direction());
		float halfB = glm::dot(oc, ray.Direction());
		float c = glm::length2(oc) - m_Radius * m_Radius;
		float discriminant = halfB * halfB - a * c;

		if (discriminant < 0)
			return false;

		float sqrtDiscriminant = sqrt(discriminant);
		float root = (-halfB - sqrtDiscriminant) / a;

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

		glm::vec3 outwardNormal = (hit.Point - m_Center) / m_Radius;
		hit.SetFaceNormal(ray, outwardNormal);

		return true;
	}

private:
	glm::vec3 m_Center;
	float m_Radius;
	std::shared_ptr<Material> m_Material;
	bool m_IsMoving;
	glm::vec3 m_MoveVector;

	glm::vec3 Center(float time) const
	{
		return m_Center + time * m_MoveVector;
	}
};
