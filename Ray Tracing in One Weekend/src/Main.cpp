#include "Utils.h"

#include "Camera.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Color.h"
#include "Material.h"

int main()
{
	std::shared_ptr<Lambertian> groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));

	HittableList world;
	world.Add(std::make_shared<Sphere>(Vec3(0, -1000, -1), 1000, groundMaterial));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			double chooseMat = RandomDouble();
			Vec3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

			if ((center - Vec3(4, 0.2, 0)).length() > 0.9)
			{
				std::shared_ptr<Material> sphereMaterial;

				if (chooseMat < 0.8)
				{
					Color albedo = Color::Random() * Color::Random();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else if (chooseMat < 0.95)
				{
					Color albedo = Color::Random(0.5, 1);
					double fuzz = RandomDouble(0, 0.5);
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else
				{
					sphereMaterial = std::make_shared<Dielectric>(1.5);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
			}
		}
	}

	std::shared_ptr<Material> dielectric = std::make_shared<Dielectric>(1.5);
	world.Add(std::make_shared<Sphere>(Vec3(0, 1, 0), 1, dielectric));

	std::shared_ptr<Material> lambertian = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.Add(std::make_shared<Sphere>(Vec3(-4, 1, 0), 1, lambertian));

	std::shared_ptr<Material> metal = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0);
	world.Add(std::make_shared<Sphere>(Vec3(4, 1, 0), 1, metal));

	Camera camera;

#if 0 // High res toggle
	camera.ImageWidth = 1920;
	camera.ImageHeight = 1080;
#else
	camera.ImageWidth = 400;
	camera.ImageHeight = 225;
#endif

	camera.SamplesPerPixel = 100;
	camera.MaxBounces = 10;

	camera.VerticalFOV = 20;
	camera.LookFrom = Vec3(13, 2, 3);
	camera.LookAt = Vec3(0, 0, 0);
	camera.ViewUp = Vec3(0, 1, 0);

	camera.DefocusAngle = 0.6;
	camera.FocusDistance = 10;

	camera.Render(world);
}
