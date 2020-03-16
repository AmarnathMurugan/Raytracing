
#include "Headers/Hitable_list.h"
#include "Headers/Sphere.h"

Vector3 ColorAtRay(const Ray& ray, HitableList& world ,HitRecord& hitRecord);
 

int main()
{
	std::cout << "P3\n";
	int Width = 200;
	int Height = 100;
	std::cout << Width << " " << Height << "\n255\n";

	Vector3 LowerLeftCorner(-2, -1, -1);
	Vector3 Horizontal(4, 0, 0);
	Vector3 Vertical(0, 2, 0);
	Vector3 Origin(0, 0, 0);

	Hitable* ModelArrays[2];
	ModelArrays[0] = new Sphere(Vector3(0, 0, -1), 0.5);
	ModelArrays[1] = new Sphere(Vector3(0, -100.5, -1), 100);
	HitableList World(ModelArrays, 2);
	HitRecord hitRecord;
	for (int y = Height - 1; y >= 0; y--)
	{
		for (int x = 0; x < Width; x++)
		{
			float U = x / (float)Width;
			float V = y / (float)Height;
			Ray ray(Origin, LowerLeftCorner + U*Horizontal + V * Vertical);
			Vector3 color = ColorAtRay(ray,World, hitRecord);
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


