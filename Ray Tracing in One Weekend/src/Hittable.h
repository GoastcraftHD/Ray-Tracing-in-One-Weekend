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
	float U;
	float V;
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

class Translate : public Hittable
{
public:
	Translate(std::shared_ptr<Hittable> p, const glm::vec3& displacement)
		: m_Object(p), m_Offset(displacement)
	{
		m_Bbox = m_Object->BoundingBox() + m_Offset;
	}

	bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const override
	{
		Ray offsetR(ray.Origin() - m_Offset, ray.Direction(), ray.Time());

		if (!m_Object->Hit(offsetR, rayT, hit))
			return false;

		hit.Point += m_Offset;

		return true;
	}

	AABB BoundingBox() const override { return m_Bbox; }

private:
	std::shared_ptr<Hittable> m_Object;
	glm::vec3 m_Offset;
	AABB m_Bbox;
};

class RotateY : public Hittable
{
public:
	RotateY(std::shared_ptr<Hittable> p, float angle)
		: m_Object(p)
	{
		float radians = glm::radians(angle);
		m_SinTheta = sin(radians);
		m_CosTheta = cos(radians);
		m_Bbox = m_Object->BoundingBox();

		glm::vec3 min(Infinity, Infinity, Infinity);
		glm::vec3 max(-Infinity, -Infinity, -Infinity);

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					float x = i * m_Bbox.X.Max + (1 - i) * m_Bbox.X.Min;
					float y = j * m_Bbox.Y.Max + (1 - j) * m_Bbox.Y.Min;
					float z = k * m_Bbox.Z.Max + (1 - k) * m_Bbox.Z.Min;

					float newX = m_CosTheta * x + m_SinTheta * z;
					float newZ = -m_SinTheta * x + m_CosTheta * z;

					glm::vec3 tester(newX, y, newZ);

					for (int c = 0; c < 3; c++)
					{
						min[c] = fmin(min[c], tester[c]);
						max[c] = fmax(max[c], tester[c]);
					}
				}
			}
		}

		m_Bbox = AABB(min, max);
	}

	bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const override
	{
		glm::vec3 origin = ray.Origin();
		glm::vec3 direction = ray.Direction();

		origin.x = m_CosTheta * ray.Origin().x - m_SinTheta * ray.Origin().z;
		origin.z = m_SinTheta * ray.Origin().x + m_CosTheta * ray.Origin().z;

		direction.x = m_CosTheta * ray.Direction().x - m_SinTheta * ray.Direction().z;
		direction.z = m_SinTheta * ray.Direction().x + m_CosTheta * ray.Direction().z;

		Ray rotatedRay(origin, direction, ray.Time());

		if (!m_Object->Hit(rotatedRay, rayT, hit))
			return false;

		glm::vec3 point = hit.Point;
		point.x = m_CosTheta * hit.Point.x + m_SinTheta * hit.Point.z;
		point.z = -m_SinTheta * hit.Point.x + m_CosTheta * hit.Point.z;

		glm::vec3 normal = hit.Normal;
		normal.x = m_CosTheta * hit.Normal.x + m_SinTheta * hit.Normal.z;
		normal.z = -m_SinTheta * hit.Normal.x + m_CosTheta * hit.Normal.z;

		hit.Point = point;
		hit.Normal = normal;

		return true;
	}

	AABB BoundingBox() const override { return m_Bbox; }

private:
	std::shared_ptr<Hittable> m_Object;
	float m_SinTheta;
	float m_CosTheta;
	AABB m_Bbox;
};
