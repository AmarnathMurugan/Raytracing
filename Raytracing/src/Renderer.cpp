#include <time.h>
#include "Headers/Hitable_list.h"
#include "Headers/Sphere.h"
#include "Headers/Camera.h"
#include "Headers/Material.h"
#include <fstream>
Vector3 ColorAtRay(const Ray& ray, HitableList& world, int depth);

Vector3 RandomPointInUnitSphere();
unsigned long int count = 0;



int main()
{
	//srand((unsigned)time(0));
	const int Width = 200;
	const int Height = 100;
	const int Samples = 100;
	const int MaxDepth = 50;
	std::cout << "P3\n";
	std::cout << Width << " " << Height << "\n255\n";
	Vector3 LookFrom(0, 0, 0);
	Vector3 LookAt(0, 0, -1);
	Vector3 ViewUp(0, 1, 0);
	double focalDistance = (LookFrom - LookAt).length();
	double aperture = 0.0;
	Camera cam(90,(Width/(double)Height),aperture,focalDistance,LookFrom,LookAt,ViewUp);
	
	HitableList World(make_shared<Sphere>(Vector3(0, 0, -1), 0.5));
	World.Add(make_shared<Sphere>(Vector3(0, -100.5, -1), 100));
	
	for (int y = Height - 1; y >= 0; --y)
	{
		for (int x = 0; x < Width; ++x)
		{			
			std::cerr << "\rPercent Complete : " << ((Width)*(Height-y-1)+(x+1))*100/(float)(Width*Height) << "  x : " << x << "  y : " << y << ' ' << std::flush;
			 
			Vector3 color(0,0,0);

			for (int s = 0; s < Samples; s++)
			{
				double U = (x + RandomDouble()) / (double)Width;
				double V = (y + RandomDouble()) / (double)Height;
				Ray ray = cam.GetRayAtUV(U, V);
				color += ColorAtRay(ray, World, MaxDepth);
			}
			color /= (double)Samples;
			color = Vector3(sqrt(color.r()), sqrt(color.g()), sqrt(color.b()));
		 
			int r = static_cast<int>(256 *  Clamp(color.r(),0,0.999));
			int g = static_cast<int>(256 *  Clamp(color.g(),0,0.999));
			int b = static_cast<int>(256 *  Clamp(color.b(),0,0.999));
			std::cout << r << " " << g << " " << b<<"  ";				 
		}
		std::cout << std::endl;		 
	}

	std::cerr << "\n DONE";
	std::cin.get();
}

Vector3 ColorAtRay(const Ray& ray, HitableList& world, int depth)
{
	if (depth <= 0)
		return Vector3(0, 0, 0);

	HitRecord hitRecord;
	if (world.isHit(ray, 0.001, DBL_MAX, hitRecord))
	{
		Vector3 target = hitRecord.HitPoint + hitRecord.Normal + RandomPointInUnitSphere();
		return 0.5 * ColorAtRay(Ray(hitRecord.HitPoint,(target-hitRecord.HitPoint)),world,depth-1);
		/*Vector3 Attenuation;
		Ray ScatteredRay;
		if (depth < 50 && hitRecord.mat_ptr->scatter(ray, hitRecord, Attenuation, ScatteredRay))
			return Attenuation * ColorAtRay(ScatteredRay, world, depth + 1);
		else
			return Vector3(0, 0, 0);*/
	}
	else
	{
		Vector3 NormalizedDirection = ray.Ray_Direction().normalized();
		double NormalizedY = 0.5*(NormalizedDirection.y() + 1);
		return (1 - NormalizedY)*Vector3(1, 1, 1) + NormalizedY * Vector3(0.5, 0.7, 1.0);
	}
}





