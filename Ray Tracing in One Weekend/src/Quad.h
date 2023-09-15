#pragma once

#include <cmath>

#include "Utils.h"
#include "Hittable.h"

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
