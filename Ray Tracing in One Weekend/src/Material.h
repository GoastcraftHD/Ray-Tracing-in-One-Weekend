#pragma once

#include "Color.h"
#include "Hittable.h"
#include "Utils.h"

struct HitRecord;

class Material
{
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& inRay, const HitRecord& hit, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const Color& albedo) : m_Albedo(albedo) {}

	bool Scatter(const Ray& inRay, const HitRecord& hit, Color& attenuation, Ray& scattered) const override
	{
		Vec3 scatterDirection = hit.Normal + RandomUnitVector();

		if (scatterDirection.NearZero())
			scatterDirection = hit.Normal;

		scattered = Ray(hit.Point, scatterDirection);
		attenuation = m_Albedo;

		return true;
	}

private:
	Color m_Albedo;
};

class Metal : public Material
{
public:
	Metal(const Color& albedo, double fuzz) : m_Albedo(albedo), m_Fuzz(fuzz) {}

	bool Scatter(const Ray& inRay, const HitRecord& hit, Color& attenuation, Ray& scattered) const override
	{
		Vec3 reflected = Reflect(UnitVector(inRay.Direction()), hit.Normal);
		scattered = Ray(hit.Point, reflected + m_Fuzz * RandomUnitVector());
		attenuation = m_Albedo;

		return (Dot(scattered.Direction(), hit.Normal) > 0);
	}

private:
	Color m_Albedo;
	double m_Fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(double indexOfRefraction) : m_IOR(indexOfRefraction) {}

	bool Scatter(const Ray& inRay, const HitRecord& hit, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1, 1, 1);
		double refractionRatio = hit.FrontFace ? (1.0 / m_IOR) : m_IOR;

		Vec3 unitDirection = UnitVector(inRay.Direction());
		double cosTheta = fmin(Dot(-unitDirection, hit.Normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		Vec3 direction;

		if (cannotRefract || Reflectance(cosTheta, refractionRatio) > RandomDouble())
			direction = Reflect(unitDirection, hit.Normal);
		else
			direction = Refract(unitDirection, hit.Normal, refractionRatio);

		scattered = Ray(hit.Point, direction);

		return true;
	}

private:
	double m_IOR;

	static double Reflectance(double cosine, double refIdx)
	{
		double r0 = (1 - refIdx) / (1 + refIdx);
		r0 = r0 * r0;

		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
