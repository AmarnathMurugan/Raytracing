
#include "Headers/Hitable_list.h"
#include "Headers/Sphere.h"
#include "Headers/Camera.h"
#include <time.h>

Vector3 ColorAtRay(const Ray& ray, HitableList* world  );
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
	Hitable* ModelArrays[2];
	ModelArrays[0] = new Sphere(Vector3(0, 0, -1), 0.5);
	ModelArrays[1] = new Sphere(Vector3(0, -100.5, -1), 100);
	HitableList* World = new HitableList(ModelArrays, 2);
	
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
				color += ColorAtRay(ray, World);
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

Vector3 ColorAtRay(const Ray& ray, HitableList* world  )
{
	HitRecord hitRecord;
	if (world->isHit(ray, 0.01, 10, hitRecord))
	{
		Vector3 target = hitRecord.HitPoint + hitRecord.Normal + RandomPointInUnitSphere();
		return 0.5 * ColorAtRay(Ray(hitRecord.HitPoint,target - hitRecord.HitPoint), world);
	}
	else
	{
		Vector3 NormalizedDirection = ray.Ray_Direction().normalized();
		double NormalizedY = 0.5*(NormalizedDirection.y() + 1);
		return (1 - NormalizedY)*Vector3(1.0, 1.0, 1.0) + NormalizedY * Vector3(.5, .7, 1.0);
	}
}

float rand01()
{
	return ((float)rand() / RAND_MAX);
}

Vector3 RandomPointInUnitSphere()
{
	Vector3 Offset(1, 1, 1);
	Vector3 point;
	do
	{
		point = 2.0 * Vector3(rand01(), rand01(), rand01()) - Offset;
	} while (point.SqrdLength() >= 1);
	return point;
}


