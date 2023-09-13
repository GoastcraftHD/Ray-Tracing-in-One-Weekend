#pragma once

#include <iostream>
#include <vector>
#include <execution>

#include "stb_image_write.h"

#include "Utils.h"
#include "Color.h"
#include "Hittable.h"
#include "Material.h"

class Camera
{
public:
	int ImageWidth = 320;
	int ImageHeight = 240;
	int SamplesPerPixel = 10;
	int MaxBounces = 10;

	double VerticalFOV = 90;
	Vec3 LookFrom = Vec3(0, 0, -1);
	Vec3 LookAt = Vec3(0, 0, 0);
	Vec3 ViewUp = Vec3(0, 1, 0);

	double DefocusAngle = 0;
	double FocusDistance = 10;

	void Render(const Hittable& world)
	{
		Initialize();
#define MT 1

		for (int j = 0; j < ImageHeight; j++)
		{
			std::cout << "\rScanlines remaining: " << (ImageHeight - j) << " (" << (floor((double)j / (double)ImageHeight * 100 * 100) / 100) << "%)          " << std::flush;

#if MT
		std::for_each(std::execution::par, m_HorizontalIter.begin(), m_HorizontalIter.end(),
			[&](uint32_t i)
#else
			for (int i = 0; i < ImageWidth; i++)
#endif
			{
				Color pixelColor(0, 0, 0);

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

	Vec3 m_CameraCenter = Vec3(0, 0, 0);
	Vec3 m_Pixel00Location = Vec3(0, 0, 0);
	Vec3 m_PixelDeltaU = Vec3(0, 0, 0);
	Vec3 m_PixelDeltaV = Vec3(0, 0, 0);
	Vec3 m_U, m_V, m_W;
	Vec3 m_DefocusDiskU;
	Vec3 m_DefocusDiskV;

	void Initialize()
	{
		m_CameraCenter = LookFrom;

		double theta = DegreesToRadians(VerticalFOV);
		double h = tan(theta / 2);
		double viewportHeight = 2 * h * FocusDistance;
		double viewportWidth = viewportHeight * (static_cast<double>(ImageWidth) / ImageHeight);

		m_W = UnitVector(LookFrom - LookAt);
		m_U = UnitVector(Cross(ViewUp, m_W));
		m_V = Cross(m_W, m_U);

		Vec3 viewportU = viewportWidth * m_U;
		Vec3 viewportV = viewportHeight * -m_V;

		m_PixelDeltaU = viewportU / ImageWidth;
		m_PixelDeltaV = viewportV / ImageHeight;

		Vec3 viewportUpperLeft = m_CameraCenter - (FocusDistance * m_W) - viewportU / 2 - viewportV / 2;
		m_Pixel00Location = viewportUpperLeft + 0.5 * (m_PixelDeltaU + m_PixelDeltaV);

		double defocusRadius = FocusDistance * tan(DegreesToRadians(DefocusAngle / 2));
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
		Vec3 pixelCenter = m_Pixel00Location + (i * m_PixelDeltaU) + (j * m_PixelDeltaV);
		Vec3 pixelSample = pixelCenter + PixelSampleSquare();

		Vec3 rayOrigin = DefocusAngle <= 0 ? m_CameraCenter : DefocusDiskSample();
		Vec3 rayDirection = pixelSample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}

	Vec3 DefocusDiskSample()
	{
		Vec3 p = RandomInUnitDisk();
		return m_CameraCenter + (p[0] * m_DefocusDiskU) + (p[1] * m_DefocusDiskV);
	}

	Vec3 PixelSampleSquare() const
	{
		double px = -0.5 + RandomDouble();
		double py = -0.5 + RandomDouble();
		return (px * m_PixelDeltaU) + (py * m_PixelDeltaV);
	}

	Color RayColor(const Ray& ray, int depth, const Hittable& world) const
	{
		if (depth <= 0)
			return Color(0, 0, 0);

		HitRecord hit;

		if (world.Hit(ray, Interval(0.001, Infinity), hit))
		{
			Ray scattered;
			Color attenuation;

			if (hit.Material->Scatter(ray, hit, attenuation, scattered))
				return attenuation * RayColor(scattered, depth - 1, world);

			return Color(0, 0, 0);
		}

		Vec3 unit_direction = UnitVector(ray.Direction());
		double a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
	}
};