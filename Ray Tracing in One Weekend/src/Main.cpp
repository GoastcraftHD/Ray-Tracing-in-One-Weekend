#include "Utils.h"

#include "Camera.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"
#include "BVH.h"

int main()
{
	std::shared_ptr<Lambertian> groundMaterial = std::make_shared<Lambertian>(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	HittableList world;
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, -1.0f), 1000.0f, groundMaterial));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMat = RandomFloat();
			glm::vec3 center(a + 0.9f * RandomFloat(), 0.2f, b + 0.9f * RandomFloat());

			if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f)
			{
				std::shared_ptr<Material> sphereMaterial;

				if (chooseMat < 0.8f)
				{
					glm::vec4 albedo = RandomColor() * RandomColor();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					glm::vec3 toCenter = center + glm::vec3(0.0f, RandomFloat(0.0f, 0.25f), 0.0f);
					world.Add(std::make_shared<Sphere>(center, toCenter, 0.2, sphereMaterial));
				}
				else if (chooseMat < 0.95f)
				{
					glm::vec4 albedo = RandomColor(0.5f, 1.0f);
					float fuzz = RandomFloat(0.0f, 0.5f);
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
					world.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
				else
				{
					sphereMaterial = std::make_shared<Dielectric>(1.5f);
					world.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
			}
		}
	}

	std::shared_ptr<Material> dielectric = std::make_shared<Dielectric>(1.5);
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, dielectric));

	std::shared_ptr<Material> lambertian = std::make_shared<Lambertian>(glm::vec4(0.4f, 0.2f, 0.1f, 1.0f));
	world.Add(std::make_shared<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, lambertian));

	std::shared_ptr<Material> metal = std::make_shared<Metal>(glm::vec4(0.7f, 0.6f, 0.5f, 1.0f), 0);
	world.Add(std::make_shared<Sphere>(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, metal));

	world = HittableList(std::make_shared<BVHNode>(world));

	Camera camera;

#if 1 // High res toggle
	camera.ImageWidth = 1920;
	camera.ImageHeight = 1080;
#else
	camera.ImageWidth = 400;
	camera.ImageHeight = 225;
#endif

	camera.SamplesPerPixel = 1024;
	camera.MaxBounces = 10;

	camera.VerticalFOV = 20.0f;
	camera.LookFrom = glm::vec3(13.0f, 2.0f, 3.0f);
	camera.LookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.ViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.DefocusAngle = 0.6f;
	camera.FocusDistance = 10.0f;

	camera.Render(world);
}
