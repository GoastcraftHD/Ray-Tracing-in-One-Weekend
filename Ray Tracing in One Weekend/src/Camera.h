#pragma once

#include <iostream>
#include <vector>
#include <execution>

#include "stb_image_write.h"

#include "Utils.h"
#include "Hittable.h"
#include "Material.h"

class Camera
{
public:
	glm::vec3 a;
	int ImageWidth = 320;
	int ImageHeight = 240;
	int SamplesPerPixel = 10;
	int MaxBounces = 10;

	float VerticalFOV = 90;
	glm::vec3 LookFrom = glm::vec3(0, 0, -1);
	glm::vec3 LookAt = glm::vec3(0, 0, 0);
	glm::vec3 ViewUp = glm::vec3(0, 1, 0);

	float DefocusAngle = 0;
	float FocusDistance = 10;

	void Render(const Hittable& world)
	{
		Initialize();
#define MT 1

		for (int j = 0; j < ImageHeight; j++)
		{
			std::cout << "\rScanlines remaining: " << (ImageHeight - j) << " (" << (floor((float)j / (float)ImageHeight * 100 * 100) / 100) << "%)          " << std::flush;

#if MT
		std::for_each(std::execution::par, m_HorizontalIter.begin(), m_HorizontalIter.end(),
			[&](uint32_t i)
#else
			for (int i = 0; i < ImageWidth; i++)
#endif
			{
				glm::vec4 pixelColor(0.0f, 0.0f, 0.0f, 1.0f);

				for (int sample = 0; sample < SamplesPerPixel; sample++)
				{
					Ray ray = GetRay(i, j);
					pixelColor += RayColor(ray, MaxBounces, world);
				}

				uint32_t packedColor = PackColor(pixelColor, SamplesPerPixel);
				m_Data[j * ImageWidth + i] = packedColor;
#if MT
			});
#else
			}
#endif

		}

		stbi_write_png("C:/dev/VisualStudio/Ray Tracing in One Weekend/Ray Tracing in One Weekend/image.png",  ImageWidth, ImageHeight, 4, m_Data, ImageWidth * 4);

		std::cout << "\rDone.                           \n";
	}

private:
	uint32_t* m_Data;
	std::vector<uint32_t> m_HorizontalIter;

	glm::vec3 m_CameraCenter = glm::vec3(0, 0, 0);
	glm::vec3 m_Pixel00Location = glm::vec3(0, 0, 0);
	glm::vec3 m_PixelDeltaU = glm::vec3(0, 0, 0);
	glm::vec3 m_PixelDeltaV = glm::vec3(0, 0, 0);
	glm::vec3 m_U, m_V, m_W;
	glm::vec3 m_DefocusDiskU;
	glm::vec3 m_DefocusDiskV;

	void Initialize()
	{
		m_CameraCenter = LookFrom;

		float theta = glm::radians(VerticalFOV);
		float h = tan(theta / 2);
		float viewportHeight = 2 * h * FocusDistance;
		float viewportWidth = viewportHeight * (static_cast<float>(ImageWidth) / ImageHeight);

		m_W = glm::normalize(LookFrom - LookAt);
		m_U = glm::normalize(glm::cross(ViewUp, m_W));
		m_V = glm::cross(m_W, m_U);

		glm::vec3 viewportU = viewportWidth * m_U;
		glm::vec3 viewportV = viewportHeight * -m_V;

		m_PixelDeltaU = viewportU / (float)ImageWidth;
		m_PixelDeltaV = viewportV / (float)ImageHeight;

		glm::vec3 viewportUpperLeft = m_CameraCenter - (FocusDistance * m_W) - viewportU / 2.0f - viewportV / 2.0f;
		m_Pixel00Location = viewportUpperLeft + 0.5f * (m_PixelDeltaU + m_PixelDeltaV);

		float defocusRadius = FocusDistance * tan(glm::radians(DefocusAngle / 2));
		m_DefocusDiskU = m_U * defocusRadius;
		m_DefocusDiskV = m_V * defocusRadius;

		m_HorizontalIter.resize(ImageWidth);

		m_Data = new uint32_t[ImageWidth * ImageHeight];

		for (uint32_t i = 0; i < ImageWidth; i++)
		{
			m_HorizontalIter[i] = i;
		}
	}

	Ray GetRay(int i, int j)
	{
		glm::vec3 pixelCenter = m_Pixel00Location + ((float)i * m_PixelDeltaU) + ((float)j * m_PixelDeltaV);
		glm::vec3 pixelSample = pixelCenter + PixelSampleSquare();

		glm::vec3 rayOrigin = DefocusAngle <= 0.0f ? m_CameraCenter : DefocusDiskSample();
		glm::vec3 rayDirection = pixelSample - rayOrigin;
		float rayTime = RandomFloat();

		return Ray(rayOrigin, rayDirection, rayTime);
	}

	glm::vec3 DefocusDiskSample()
	{
		glm::vec3 p = RandomInUnitDisk();
		return m_CameraCenter + (p.x * m_DefocusDiskU) + (p.y * m_DefocusDiskV);
	}

	glm::vec3 PixelSampleSquare() const
	{
		float px = -0.5 + RandomFloat();
		float py = -0.5 + RandomFloat();
		return (px * m_PixelDeltaU) + (py * m_PixelDeltaV);
	}

	glm::vec4 RayColor(const Ray& ray, int depth, const Hittable& world) const
	{
		if (depth <= 0)
			return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		HitRecord hit;

		if (world.Hit(ray, Interval(0.001, Infinity), hit))
		{
			Ray scattered;
			glm::vec4 attenuation;

			if (hit.Material->Scatter(ray, hit, attenuation, scattered))
				return attenuation * RayColor(scattered, depth - 1, world);

			return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		glm::vec3 unit_direction = glm::normalize(ray.Direction());
		float a = 0.5 * (unit_direction.y + 1.0);
		return (1.0f - a) * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) + a * glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
	}

	uint32_t PackColor(const glm::vec4& pixelColor, int samplesPerPixel)
	{
		float r = pixelColor.r;
		float g = pixelColor.g;
		float b = pixelColor.b;
		float a = pixelColor.a;

		float scale = 1.0f / samplesPerPixel;
		r *= scale;
		g *= scale;
		b *= scale;
		a *= scale;

		r = sqrt(r);
		g = sqrt(g);
		b = sqrt(b);
		a = sqrt(a);

		static const Interval intensity(0.0, 0.999);

		uint32_t packedColor = 0;
		packedColor |= static_cast<uint8_t>(256 * intensity.Clamp(a)) << 24;
		packedColor |= static_cast<uint8_t>(256 * intensity.Clamp(b)) << 16;
		packedColor |= static_cast<uint8_t>(256 * intensity.Clamp(g)) << 8;
		packedColor |= static_cast<uint8_t>(256 * intensity.Clamp(r));

		return packedColor;
	}
};
