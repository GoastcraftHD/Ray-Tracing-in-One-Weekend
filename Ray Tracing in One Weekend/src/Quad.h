#pragma once

#include "Utils.h"
#include "Hittable.h"

class Quad : public Hittable
{
public:
	Quad(const glm::vec3& q, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> material)
		: m_Q(q), m_U(u), m_V(v), m_Material(material)
	{
		SetBoundingBox();
	}

	virtual void SetBoundingBox()
	{
		m_Bbox = AABB(m_Q, m_Q + m_V + m_V).Pad();
	}

	AABB BoundingBox() const override { return m_Bbox; }

	bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const override
	{
		return false;
	}

private:
	glm::vec3 m_Q;
	glm::vec3 m_U, m_V;
	std::shared_ptr<Material> m_Material;
	AABB m_Bbox;
};
