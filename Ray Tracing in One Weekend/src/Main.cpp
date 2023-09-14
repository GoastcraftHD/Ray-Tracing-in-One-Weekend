#include "Utils.h"

#include "Camera.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"
#include "BVH.h"
#include "Texture.h"

void RandomSpheres(Camera camera)
{
	std::shared_ptr<Lambertian> groundMaterial = std::make_shared<Lambertian>(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	HittableList world;
	std::shared_ptr<CheckerTexture> checker = std::make_shared<CheckerTexture>(0.32f, glm::vec4(0.2f, 0.3f, 0.1f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, -1.0f), 1000.0f, std::make_shared<Lambertian>(checker)));

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

	camera.VerticalFOV = 20.0f;
	camera.LookFrom = glm::vec3(13.0f, 2.0f, 3.0f);
	camera.LookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.ViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.DefocusAngle = 0.6f;
	camera.FocusDistance = 10.0f;

	camera.Render(world);
}

void TwoSpheres(Camera camera)
{
	HittableList world;

	std::shared_ptr<CheckerTexture> checker = std::make_shared<CheckerTexture>(0.8f, glm::vec4(0.2f, 0.3f, 0.1f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));

	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -10.0f, 0.0f), 10.0f, std::make_shared<Lambertian>(checker)));
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 10.0f, 0.0f), 10.0f, std::make_shared<Lambertian>(checker)));

	camera.VerticalFOV = 20.0f;
	camera.LookFrom = glm::vec3(13.0f, 2.0f, 3.0f);
	camera.LookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.ViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.DefocusAngle = 0.0f;

	camera.Render(world);
}

void Earth(Camera camera)
{
	std::shared_ptr<ImageTexture> earthTexture = std::make_shared<ImageTexture>("assets/textures/earthmap.jpg");
	std::shared_ptr<Material> earthMaterial = std::make_shared<Lambertian>(earthTexture);
	std::shared_ptr<Sphere> globe = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, earthMaterial);

	camera.VerticalFOV = 20.0f;
	camera.LookFrom = glm::vec3(0.0f, 0.0f, 12.0f);
	camera.LookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.ViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.DefocusAngle = 0.0f;

	camera.Render(HittableList(globe));
}

void TwoPerlinSpheres(Camera camera)
{
	HittableList world;

	std::shared_ptr<NoiseTexture> pertext = std::make_shared<NoiseTexture>(3.0f);
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(pertext)));
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 2.0f, 0.0f), 2.0f, std::make_shared<Lambertian>(pertext)));

	camera.VerticalFOV = 20.0f;
	camera.LookFrom = glm::vec3(13.0f, 2.0f, 3.0f);
	camera.LookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.ViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.DefocusAngle = 0.0f;
	
	camera.Render(world);
}

int main()
{
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

	switch (4)
	{
		case 1: RandomSpheres(camera); break;
		case 2: TwoSpheres(camera); break;
		case 3: Earth(camera); break;
		case 4: TwoPerlinSpheres(camera); break;
	}
}
