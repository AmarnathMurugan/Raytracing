
#include "Headers/Hitable_list.h"
#include "Headers/Sphere.h"
#include "Headers/Camera.h"
#include <stdlib.h>
Vector3 ColorAtRay(const Ray& ray, HitableList& world ,HitRecord& hitRecord);
 

int main()
{
	int Width = 200;
	int Height = 100;
	int Samples = 100;
	std::cout << "P3\n";
	std::cout << Width << " " << Height << "\n255\n";
	Camera cam;	
	Hitable* ModelArrays[2];
	ModelArrays[0] = new Sphere(Vector3(0, 0, -1), 0.5);
	ModelArrays[1] = new Sphere(Vector3(0, -100.5, -1), 100);
	HitableList World(ModelArrays, 2);
	HitRecord hitRecord;
	for (int y = Height - 1; y >= 0; y--)
	{
		for (int x = 0; x < Width; x++)
		{
			Vector3 color;
			for (int s = 0; s < Samples; s++)
			{
				float U = (x) / (float)Width;
				float V = (y) / (float)Height;
				Ray ray = cam.GetRayAtUV(U, V);
				color = ColorAtRay(ray, World, hitRecord);
			}
			int r = (int)(255.99 * color.r());
			int g = (int)(255.99 * color.g());
			int b = (int)(255.99 * color.b());
			std::cout << r << " " << g << " " << b<<"  ";
			 
		}
		std::cout << std::endl;
	}


	std::cin.get();
}

Vector3 ColorAtRay(const Ray& ray, HitableList& world  ,HitRecord& hitRecord)
{

	if (world.isHit(ray, 0, 100, hitRecord))
	{
		return 0.5 * Vector3(hitRecord.Normal.r()+1, hitRecord.Normal.g() + 1, hitRecord.Normal.b() + 1);
	}
	
	Vector3 NormalizedDirection = ray.Ray_Direction().normalized();
	double NormalizedY = 0.5*(NormalizedDirection.y() + 1);
	return (1 - NormalizedY)*Vector3(1.0, 1.0, 1.0) + NormalizedY * Vector3(.5, .7, 1.0);

}


