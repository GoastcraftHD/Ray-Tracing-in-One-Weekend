#pragma once

#include <cmath>

#include "Utils.h"
#include "Hittable.h"
#include "HittableList.h"

class Quad : public Hittable
{
public:
	Quad(const glm::vec3& q, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> material)
		: m_Q(q), m_U(u), m_V(v), m_Material(material)
	{
		glm::vec3 n = glm::cross(m_U, m_V);
		m_Normal = glm::normalize(n);
		m_D = glm::dot(m_Normal, m_Q);
		m_W = n / glm::dot(n, n);

		SetBoundingBox();
	}

	virtual void SetBoundingBox()
	{
		m_Bbox = AABB(m_Q, m_Q + m_V + m_V).Pad();
	}

	AABB BoundingBox() const override { return m_Bbox; }

	bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const override
	{
		float denominator = glm::dot(m_Normal, ray.Direction());

		if (fabs(denominator) < 1e-8)
			return false;

		float t = (m_D - glm::dot(m_Normal, ray.Origin())) / denominator;

		if (!rayT.Constains(t))
			return false;

		glm::vec3 intersection = ray.At(t);
		glm::vec3 planarHitpointVector = intersection - m_Q;
		float alpha = glm::dot(m_W, glm::cross(planarHitpointVector, m_V));
		float beta = glm::dot(m_W, glm::cross(m_U, planarHitpointVector));

		if (!IsInterior(alpha, beta, hit))
			return false;

		hit.T = t;
		hit.Point = intersection;
		hit.Material = m_Material;
		hit.SetFaceNormal(ray, m_Normal);

		return true;
	}

	virtual bool IsInterior(float alpha, float beta, HitRecord& hit) const
	{
		if (alpha < 0 || 1 < alpha || beta < 0 || 1 < beta)
			return false;

		hit.U = alpha;
		hit.V = beta;

		return true;
	}

private:
	glm::vec3 m_Q;
	glm::vec3 m_U, m_V;
	std::shared_ptr<Material> m_Material;
	AABB m_Bbox;
	glm::vec3 m_Normal;
	float m_D;
	glm::vec3 m_W;
};

inline std::shared_ptr<Hittable> Box(const glm::vec3& a, const glm::vec3& b, std::shared_ptr<Material> material)
{
	std::shared_ptr<HittableList> sides = std::make_shared<HittableList>();

	glm::vec3 min = glm::vec3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
	glm::vec3 max = glm::vec3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

	glm::vec3 dx = glm::vec3(max.x - min.x, 0.0f, 0.0f);
	glm::vec3 dy = glm::vec3(0.0f, max.y - min.y, 0.0f);
	glm::vec3 dz = glm::vec3(0.0f, 0.0f, max.z - min.z);

	sides->Add(std::make_shared<Quad>(glm::vec3(min.x, min.y, max.z), dx, dy, material));
	sides->Add(std::make_shared<Quad>(glm::vec3(max.x, min.y, max.z), -dz, dy, material));
	sides->Add(std::make_shared<Quad>(glm::vec3(max.x, min.y, min.z), -dx, dy, material));
	sides->Add(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dz, dy, material));
	sides->Add(std::make_shared<Quad>(glm::vec3(min.x, max.y, max.z), dx, -dz, material));
	sides->Add(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dx, dz, material));

	return sides;
}
