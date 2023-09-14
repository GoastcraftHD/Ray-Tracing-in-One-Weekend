#pragma once

#include "Hittable.h"
#include "Utils.h"
#include "Texture.h"

struct HitRecord;

class Material
{
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& inRay, const HitRecord& hit, glm::vec4& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const glm::vec4& albedo) : m_Albedo(std::make_shared<SolidColorTexture>(albedo)) {}
	Lambertian(std::shared_ptr<Texture> texture) : m_Albedo(texture) {}

	bool Scatter(const Ray& inRay, const HitRecord& hit, glm::vec4& attenuation, Ray& scattered) const override
	{
		glm::vec3 scatterDirection = hit.Normal + RandomUnitVector();

		if (NearZero(scatterDirection))
			scatterDirection = hit.Normal;

		scattered = Ray(hit.Point, scatterDirection, inRay.Time());
		attenuation = m_Albedo->Value(hit.U, hit.V, hit.Point);

		return true;
	}

private:
	std::shared_ptr<Texture> m_Albedo;
};

class Metal : public Material
{
public:
	Metal(const glm::vec4& albedo, float fuzz) : m_Albedo(albedo), m_Fuzz(fuzz) {}

	bool Scatter(const Ray& inRay, const HitRecord& hit, glm::vec4& attenuation, Ray& scattered) const override
	{
		glm::vec3 reflected = glm::reflect(glm::normalize(inRay.Direction()), hit.Normal);
		scattered = Ray(hit.Point, reflected + m_Fuzz * RandomUnitVector(), inRay.Time());
		attenuation = m_Albedo;

		return glm::dot(scattered.Direction(), hit.Normal) > 0.0f;
	}

private:
	glm::vec4 m_Albedo;
	float m_Fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(float indexOfRefraction) : m_IOR(indexOfRefraction) {}

	bool Scatter(const Ray& inRay, const HitRecord& hit, glm::vec4& attenuation, Ray& scattered) const override
	{
		attenuation = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float refractionRatio = hit.FrontFace ? (1.0f / m_IOR) : m_IOR;

		glm::vec3 unitDirection = glm::normalize(inRay.Direction());
		float cosTheta = fmin(glm::dot(-unitDirection, hit.Normal), 1.0f);
		float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0f;
		glm::vec3 direction;

		if (cannotRefract || Reflectance(cosTheta, refractionRatio) > RandomFloat())
			direction = glm::reflect(unitDirection, hit.Normal);
		else
			direction = glm::refract(unitDirection, hit.Normal, refractionRatio);

		scattered = Ray(hit.Point, direction, inRay.Time());

		return true;
	}

private:
	float m_IOR;

	static float Reflectance(double cosine, float refIdx)
	{
		float r0 = (1.0f - refIdx) / (1.0f + refIdx);
		r0 = r0 * r0;

		return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
	}
};
