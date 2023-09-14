#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) : m_Center(center), m_Radius(radius), m_Material(material), m_IsMoving(false)
	{
		glm::vec3 rvec = glm::vec3(radius, radius, radius);
		m_Bbox = AABB(m_Center - rvec, m_Center + rvec);
	}

	Sphere(const glm::vec3& fromCenter, const glm::vec3& toCenter, float radius, std::shared_ptr<Material> material) : m_Center(fromCenter), m_Radius(radius), m_Material(material), m_IsMoving(true)
	{
		glm::vec3 rvec = glm::vec3(radius, radius, radius);
		AABB box1(fromCenter - rvec, fromCenter + rvec);
		AABB box2(toCenter - rvec, toCenter + rvec);
		m_Bbox = AABB(box1, box2);

		m_MoveVector = toCenter - fromCenter;
	}

	AABB BoundingBox() const override
	{
		return m_Bbox;
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
		GetSphereUV(outwardNormal, hit.U, hit.V);

		return true;
	}

private:
	glm::vec3 m_Center;
	float m_Radius;
	std::shared_ptr<Material> m_Material;
	bool m_IsMoving;
	glm::vec3 m_MoveVector;
	AABB m_Bbox;

	glm::vec3 Center(float time) const
	{
		return m_Center + time * m_MoveVector;
	}

	static void GetSphereUV(const glm::vec3& point, float& u, float& v)
	{
		float theta = acos(-point.y);
		float phi = atan2(-point.z, point.x) + Pi;

		u = phi / (2 * Pi);
		v = theta / Pi;
	}
};
