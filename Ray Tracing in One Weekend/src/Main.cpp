#include "Utils.h"

#include "Camera.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Quad.h"
#include "Material.h"
#include "BVH.h"
#include "Texture.h"
#include "ConstantMedium.h"

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

	camera.BackgroundColor = glm::vec4(0.7f, 0.8f, 1.0f, 1.0f);

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

	camera.BackgroundColor = glm::vec4(0.7f, 0.8f, 1.0f, 1.0f);

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

	camera.BackgroundColor = glm::vec4(0.7f, 0.8f, 1.0f, 1.0f);

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

	camera.BackgroundColor = glm::vec4(0.7f, 0.8f, 1.0f, 1.0f);
	
	camera.Render(world);
}

void Quads(Camera camera)
{
	HittableList world;

	std::shared_ptr<Material> leftRed = std::make_shared<Lambertian>(glm::vec4(1.0f, 0.2f, 0.2f, 1.0f));
	std::shared_ptr<Material> backGreen = std::make_shared<Lambertian>(glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
	std::shared_ptr<Material> rightBlue = std::make_shared<Lambertian>(glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
	std::shared_ptr<Material> upperOrange = std::make_shared<Lambertian>(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
	std::shared_ptr<Material> lowerTeal = std::make_shared<Lambertian>(glm::vec4(0.2f, 0.8f, 0.8f, 1.0f));

	world.Add(std::make_shared<Quad>(glm::vec3(-3.0f, -2.0f, 5.0f), glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(0.0f, 4.0f, 0.0f), leftRed));
	world.Add(std::make_shared<Quad>(glm::vec3(-2.0f, -2.0f, 0.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 4.0f, 0.0f), backGreen));
	world.Add(std::make_shared<Quad>(glm::vec3(3.0f, -2.0f, 1.0f), glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 4.0f, 0.0f), rightBlue));
	world.Add(std::make_shared<Quad>(glm::vec3(-2.0f, 3.0f, 1.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 4.0f), upperOrange));
	world.Add(std::make_shared<Quad>(glm::vec3(-2.0f, -3.0f, 5.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -4.0f), lowerTeal));

	camera.VerticalFOV = 80.0f;
	camera.LookFrom = glm::vec3(0.0f, 0.0f, 9.0f);
	camera.LookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.ViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.DefocusAngle = 0.0f;

	camera.BackgroundColor = glm::vec4(0.7f, 0.8f, 1.0f, 1.0f);

	camera.Render(world);
}

void SimpleLight(Camera camera)
{
	HittableList world;

	std::shared_ptr<Texture> pertext = std::make_shared<NoiseTexture>(4.0f);
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(pertext)));
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 2.0f, 0.0f), 2.0f, std::make_shared<Lambertian>(pertext)));

	std::shared_ptr<Material> diffLight = std::make_shared<DiffuseLight>(glm::vec4(4.0f, 4.0f, 4.0f, 1.0f));
	world.Add(std::make_shared<Quad>(glm::vec3(3.0f, 1.0f, -2.0f), glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), diffLight));

	camera.VerticalFOV = 20.0f;
	camera.LookFrom = glm::vec3(26.0f, 3.0f, 6.0f);
	camera.LookAt = glm::vec3(0.0f, 2.0f, 0.0f);
	camera.ViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.DefocusAngle = 0.0f;

	camera.BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	camera.Render(world);
}

void CornellBox(Camera camera)
{
	HittableList world;

	std::shared_ptr<Material> red = std::make_shared<Lambertian>(glm::vec4(0.65f, 0.05f, 0.05f, 1.0f));
	std::shared_ptr<Material> white = std::make_shared<Lambertian>(glm::vec4(0.73f, 0.73f, 0.73f, 1.0f));
	std::shared_ptr<Material> green = std::make_shared<Lambertian>(glm::vec4(0.12f, 0.45f, 0.15f, 1.0f));
	std::shared_ptr<Material> light = std::make_shared<DiffuseLight>(glm::vec4(15.0f, 15.0f, 15.0f, 1.0f));

	world.Add(std::make_shared<Quad>(glm::vec3(555.0f, 0.0f, 0.0f), glm::vec3(0.0f, 555.0f, 0.0f), glm::vec3(0.0f, 0.0f, 555.0f), green));
	world.Add(std::make_shared<Quad>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 555.0f, 0.0f), glm::vec3(0.0f, 0.0f, 555.0f), red));
	world.Add(std::make_shared<Quad>(glm::vec3(343.0f, 554.0f, 332.0f), glm::vec3(-130.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 105.0f), light));
	world.Add(std::make_shared<Quad>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(555.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 555.0f), white));
	world.Add(std::make_shared<Quad>(glm::vec3(555.0f, 555.0f, 555.0f), glm::vec3(-555.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -555.0f), white));
	world.Add(std::make_shared<Quad>(glm::vec3(0.0f, 0.0f, 555.0f), glm::vec3(555.0f, 0.0f, 0.0f), glm::vec3(0.0f, 555.0f, 0.0f), white));

	std::shared_ptr<Hittable> box1 = Box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 330.0f, 165.0f), white);
	box1 = std::make_shared<RotateY>(box1, 15.0f);
	box1 = std::make_shared<Translate>(box1, glm::vec3(265.0f, 0.0f, 295.0f));
	world.Add(box1);

	std::shared_ptr<Hittable> box2 = Box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 165.0f, 165.0f), white);
	box2 = std::make_shared<RotateY>(box2, -18.0f);
	box2 = std::make_shared<Translate>(box2, glm::vec3(130.0f, 0.0f, 65.0f));
	world.Add(box2);

	camera.VerticalFOV = 40.0f;
	camera.LookFrom = glm::vec3(278.0f, 278.0f, -800.0f);
	camera.LookAt = glm::vec3(278.0f, 278.0f, 0.0f);
	camera.ViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.DefocusAngle = 0.0f;

	camera.BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	camera.Render(world);
}

void CornellSmoke(Camera camera)
{
	HittableList world;

	std::shared_ptr<Material> red = std::make_shared<Lambertian>(glm::vec4(0.65f, 0.05f, 0.05f, 1.0f));
	std::shared_ptr<Material> white = std::make_shared<Lambertian>(glm::vec4(0.73f, 0.73f, 0.73f, 1.0f));
	std::shared_ptr<Material> green = std::make_shared<Lambertian>(glm::vec4(0.12f, 0.45f, 0.15f, 1.0f));
	std::shared_ptr<Material> light = std::make_shared<DiffuseLight>(glm::vec4(7.0f, 7.0f, 7.0f, 1.0f));

	world.Add(std::make_shared<Quad>(glm::vec3(555.0f, 0.0f, 0.0f), glm::vec3(0.0f, 555.0f, 0.0f), glm::vec3(0.0f, 0.0f, 555.0f), green));
	world.Add(std::make_shared<Quad>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 555.0f, 0.0f), glm::vec3(0.0f, 0.0f, 555.0f), red));
	world.Add(std::make_shared<Quad>(glm::vec3(113.0f, 554.0f, 127.0f), glm::vec3(330.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 305.0f), light));
	world.Add(std::make_shared<Quad>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(555.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 555.0f), white));
	world.Add(std::make_shared<Quad>(glm::vec3(555.0f, 555.0f, 555.0f), glm::vec3(-555.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -555.0f), white));
	world.Add(std::make_shared<Quad>(glm::vec3(0.0f, 0.0f, 555.0f), glm::vec3(555.0f, 0.0f, 0.0f), glm::vec3(0.0f, 555.0f, 0.0f), white));

	std::shared_ptr<Hittable> box1 = Box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 330.0f, 165.0f), white);
	box1 = std::make_shared<RotateY>(box1, 15.0f);
	box1 = std::make_shared<Translate>(box1, glm::vec3(265.0f, 0.0f, 295.0f));

	std::shared_ptr<Hittable> box2 = Box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 165.0f, 165.0f), white);
	box2 = std::make_shared<RotateY>(box2, -18.0f);
	box2 = std::make_shared<Translate>(box2, glm::vec3(130.0f, 0.0f, 65.0f));

	world.Add(std::make_shared<ConstantMedium>(box1, 0.01f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	world.Add(std::make_shared<ConstantMedium>(box2, 0.01f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	camera.VerticalFOV = 40.0f;
	camera.LookFrom = glm::vec3(278.0f, 278.0f, -800.0f);
	camera.LookAt = glm::vec3(278.0f, 278.0f, 0.0f);
	camera.ViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.DefocusAngle = 0.0f;

	camera.BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	camera.Render(world);
}

void FinalScene(Camera camera)
{
	HittableList boxes1;
	std::shared_ptr<Material> ground = std::make_shared<Lambertian>(glm::vec4(0.48f, 0.83f, 0.53f, 1.0f));

	int boxesPerSide = 20;
	for (int i = 0; i < boxesPerSide; i++)
	{
		for (int j = 0; j < boxesPerSide; j++)
		{
			float w = 100.0f;
			float x0 = -1000.0f + i * w;
			float z0 = -1000.0f + j * w;
			float y0 = 0.0f;
			float x1 = x0 + w;
			float y1 = RandomFloat(1.0f, 101.0f);
			float z1 = z0 + w;

			boxes1.Add(Box(glm::vec3(x0, y0, z0), glm::vec3(x1, y1, z1), ground));
		}
	}

	HittableList world;

	world.Add(std::make_shared<BVHNode>(boxes1));

	std::shared_ptr<Material> light = std::make_shared<DiffuseLight>(glm::vec4(7.0f, 7.0f, 7.0f, 1.0f));
	world.Add(std::make_shared<Quad>(glm::vec3(123.0f, 554.0f, 147.0f), glm::vec3(300.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 265.0f), light));

	glm::vec3 center1 = glm::vec3(400.0f, 400.0f, 200.0f);
	glm::vec3 center2 = center1 + glm::vec3(30.0f, 0.0f, 0.0f);
	std::shared_ptr<Material> sphereMaterial = std::make_shared<Lambertian>(glm::vec4(0.7f, 0.3f, 0.1f, 1.0f));
	world.Add(std::make_shared<Sphere>(center1, center2, 50.0f, sphereMaterial));

	world.Add(std::make_shared<Sphere>(glm::vec3(260.0f, 150.0f, 45.0f), 50.0f, std::make_shared<Dielectric>(1.5f)));
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 150.0f, 145.0f), 50.0f, std::make_shared<Metal>(glm::vec4(0.8f, 0.8f, 0.9f, 1.0f), 1.0f)));

	std::shared_ptr<Hittable> boundary = std::make_shared<Sphere>(glm::vec3(360.0f, 150.0f, 145.0f), 70.0f, std::make_shared<Dielectric>(1.0f));
	world.Add(boundary);
	world.Add(std::make_shared<ConstantMedium>(boundary, 0.2f, glm::vec4(0.2f, 0.4f, 0.9f, 1.0f)));
	boundary = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.0f), 5000.0f, std::make_shared<Dielectric>(1.5f));
	world.Add(std::make_shared<ConstantMedium>(boundary, 0.0001f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	std::shared_ptr<Material> emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("assets/textures/earthmap.jpg"));
	world.Add(std::make_shared<Sphere>(glm::vec3(400.0f, 200.0f, 400.0f), 100.0f, emat));
	std::shared_ptr<Texture> pertext = std::make_shared<NoiseTexture>(0.1f);
	world.Add(std::make_shared<Sphere>(glm::vec3(220.0f, 280.0f, 300.0f), 80.0f, std::make_shared<Lambertian>(pertext)));

	HittableList boxes2;
	std::shared_ptr<Material> white = std::make_shared<Lambertian>(glm::vec4(0.73f, 0.73f, 0.73f, 1.0f));
	int ns = 1000;

	for (int j = 0; j < ns; j++)
	{
		boxes2.Add(std::make_shared<Sphere>(RandomVector(0.0f, 165.0f), 10.0f, white));
	}

	world.Add(std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<BVHNode>(boxes2), 15.0f), glm::vec3(-100.0f, 270.0f, 395.0f)));

	camera.VerticalFOV = 40.0f;
	camera.LookFrom = glm::vec3(479.0f, 278.0f, -600.0f);
	camera.LookAt = glm::vec3(278.0f, 278.0f, 0.0f);
	camera.ViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.DefocusAngle = 0.0f;

	camera.BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	camera.Render(world);
}

int main()
{
	Camera camera;

#if 0 // High res toggle
	camera.ImageWidth = 2000;
	camera.ImageHeight = 2000;
#else
	camera.ImageWidth = 400;
	camera.ImageHeight = 400;
#endif

	camera.SamplesPerPixel = 100000;
	camera.MaxBounces = 10;

	switch (9)
	{
		case 1: RandomSpheres(camera); break;
		case 2: TwoSpheres(camera); break;
		case 3: Earth(camera); break;
		case 4: TwoPerlinSpheres(camera); break;
		case 5: Quads(camera); break;
		case 6: SimpleLight(camera); break;
		case 7: CornellBox(camera); break;
		case 8: CornellSmoke(camera); break;
		case 9: FinalScene(camera); break;
	}
}
