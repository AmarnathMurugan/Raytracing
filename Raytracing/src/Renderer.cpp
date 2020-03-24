#include <time.h>
#include "Headers/Hitable_list.h"
#include "Headers/Sphere.h"
#include "Headers/Camera.h"
#include "Headers/Material.h"

Vector3 ColorAtRay(const Ray& ray, HitableList* world, int depth);
float rand01();
Vector3 RandomPointInUnitSphere();
unsigned long int count = 0;



int main()
{
	srand((unsigned)time(0));
	int Width = 200;
	int Height = 100;
	int Samples = 100;
	std::cout << "P3\n";
	std::cout << Width << " " << Height << "\n255\n";
	Camera cam;	
	Hitable* ModelArrays[5];
	ModelArrays[0] = new Sphere(Vector3(0, 0, -1), 0.5, new Lambertian(Vector3(0.8,0.3,0.3)));
	ModelArrays[1] = new Sphere(Vector3(1, 0, -1), 0.5, new Metal(Vector3(0.7, 0.7, 0.7),0));
	ModelArrays[2] = new Sphere(Vector3(-1, 0, -1), 0.5, new Dielectric(1.5));
	ModelArrays[3] = new Sphere(Vector3(0, -100.5, -1), 100, new Lambertian(Vector3(0.8, 0.8, 0.0)));
	ModelArrays[4] = new Sphere(Vector3(-1, 0, -1), -0.45, new Dielectric(1.5));
	HitableList* World = new HitableList(ModelArrays, 5);
	
	for (int y = Height - 1; y >= 0; y--)
	{
		for (int x = 0; x < Width; x++)
		{
			Vector3 color(0,0,0);
			for (int s = 0; s < Samples; s++)
			{
				float U = (x + rand01()) / (float)Width;
				float V = (y + rand01()) / (float)Height;
				Ray ray = cam.GetRayAtUV(U, V);
				color += ColorAtRay(ray, World, 0);
			}
			color /= (double)Samples;
			color = Vector3(sqrt(color.r()), sqrt(color.g()), sqrt(color.b()));
			int r = (int)(255.99 * color.r());
			int g = (int)(255.99 * color.g());
			int b = (int)(255.99 * color.b());
			std::cout << r << " " << g << " " << b<<"  ";			 
		}
		std::cout << std::endl;
	}

  
	std::cin.get();
}

Vector3 ColorAtRay(const Ray& ray, HitableList* world, int depth)
{
	HitRecord hitRecord;
	if (world->isHit(ray, 0.001, DBL_MAX, hitRecord))
	{
		Vector3 Attenuation;
		Ray ScatteredRay;
		if (depth < 50 && hitRecord.mat_ptr->scatter(ray, hitRecord, Attenuation, ScatteredRay))
			return Attenuation * ColorAtRay(ScatteredRay, world, depth + 1);
		else
			return Vector3(0, 0, 0);
	}
	else
	{
		Vector3 NormalizedDirection = ray.Ray_Direction().normalized();
		double NormalizedY = 0.5*(NormalizedDirection.y() + 1);
		return (1 - NormalizedY)*Vector3(1.0, 1.0, 1.0) + NormalizedY * Vector3(.5, .7, 1.0);
	}
}



