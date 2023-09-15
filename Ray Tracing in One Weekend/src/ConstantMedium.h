#pragma once

#include "Utils.h"
#include "Hittable.h"
#include "Material.h"
#include "Texture.h"

class ConstantMedium : public Hittable
{
public:
	ConstantMedium(std::shared_ptr<Hittable> b, float d, std::shared_ptr<Texture> a) : m_Boundary(b), m_NegativeInverseDensity(-1 / d), m_PhaseFunction(std::make_shared<Isotropic>(a)) {}
	ConstantMedium(std::shared_ptr<Hittable> b, float d, glm::vec4 c) : m_Boundary(b), m_NegativeInverseDensity(-1 / d), m_PhaseFunction(std::make_shared<Isotropic>(c)) {}

	bool Hit(const Ray& ray, Interval rayT, HitRecord& hit) const override
	{
		const bool enableDebug = false;
		const bool debugging = enableDebug && RandomFloat() < 0.00001f;

		HitRecord hit1, hit2;

		if (!m_Boundary->Hit(ray, Interval::Universe, hit1))
			return false;

		if (!m_Boundary->Hit(ray, Interval(hit1.T + 0.0001f, Infinity), hit2))
			return false;

		if (debugging) std::cout << "\nRayTMin=" << hit.T << ", RayTMax=" << hit2.T << "\n";

		if (hit1.T < rayT.Min) hit1.T = rayT.Min;
		if (hit2.T > rayT.Max) hit2.T = rayT.Max;

		if (hit1.T >= hit2.T)
			return false;

		if (hit1.T < 0)
			hit1.T = 0;

		float rayLength = glm::length(ray.Direction());
		float distanceInsideBoundary = (hit2.T - hit1.T) * rayLength;
		float hitDistance = m_NegativeInverseDensity * log(RandomFloat());

		if (hitDistance > distanceInsideBoundary)
			return false;

		hit.T = hit1.T + hitDistance / rayLength;
		hit.Point = ray.At(hit.T);

		if (debugging)
		{
			std::cout << "Hit Distance = " << hitDistance << "\n"
				<< "Hit T = " << hit.T << "\n"
				<< "Hit Point = " << hit.Point.x << ", " << hit.Point.y << ", " << hit.Point.z << "\n";
		}

		hit.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
		hit.FrontFace = true;
		hit.Material = m_PhaseFunction;

		return true;
	}

	AABB BoundingBox() const override { return m_Boundary->BoundingBox(); }

private:
	std::shared_ptr<Hittable> m_Boundary;
	float m_NegativeInverseDensity;
	std::shared_ptr<Material> m_PhaseFunction;
};
